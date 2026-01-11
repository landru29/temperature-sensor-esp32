#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>
#include <getopt.h>

// Buffer size for serial communication
#define BUFFER_SIZE 256
#define DEFAULT_PORT "/dev/ttyUSB0"
#define DEFAULT_BAUDRATE 9600

int serial_fd = -1;
int running = 1;

// Print help message
void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n\n", prog_name);
    printf("Options:\n");
    printf("  --port <port>        Serial port (default: %s)\n", DEFAULT_PORT);
    printf("  --bit-rate <rate>    Baud rate (default: %d)\n", DEFAULT_BAUDRATE);
    printf("  -h, --help           Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s\n", prog_name);
    printf("  %s --port /dev/ttyUSB0 --bit-rate 115200\n", prog_name);
    printf("  %s --port COM3 --bit-rate 9600\n\n", prog_name);
    printf("Supported baud rates: 9600, 19200, 38400, 57600, 115200\n");
}

// Serial port configuration
int configure_serial(const char *port, int baudrate) {
    struct termios tty;
    
    serial_fd = open(port, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        fprintf(stderr, "Port: %s\n", port);
        perror("Error opening port");
        return -1;
    }
    
    if (tcgetattr(serial_fd, &tty) != 0) {
        perror("Error tcgetattr");
        return -1;
    }
    
    // Set parameters
    speed_t speed;
    switch(baudrate) {
        case 9600: speed = B9600; break;
        case 19200: speed = B19200; break;
        case 38400: speed = B38400; break;
        case 57600: speed = B57600; break;
        case 115200: speed = B115200; break;
        default:
            fprintf(stderr, "Unsupported baud rate\n");
            return -1;
    }
    
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    
    tty.c_cflag &= ~PARENB;        // No parity
    tty.c_cflag &= ~CSTOPB;        // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;            // 8 bits
    #ifdef CRTSCTS
    tty.c_cflag &= ~CRTSCTS;       // No hardware flow control
    #endif
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver
    
    tty.c_lflag &= ~ICANON;        // Non-canonical mode
    tty.c_lflag &= ~ECHO;          // No echo
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    
    tty.c_cc[VTIME] = 1;
    tty.c_cc[VMIN] = 0;
    
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        perror("Error tcsetattr");
        return -1;
    }
    
    // Wait for Arduino to reset (DTR reset)
    usleep(2000000);
    tcflush(serial_fd, TCIOFLUSH);
    
    return 0;
}

// Thread to read data from the serial port
void *read_thread(void *arg) {
    char buffer[BUFFER_SIZE];
    int n;
    
    while (running) {
        n = read(serial_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    const char *port = DEFAULT_PORT;
    int baudrate = DEFAULT_BAUDRATE;
    
    // Options longues
    static struct option long_options[] = {
        {"port",     required_argument, 0, 'p'},
        {"bit-rate", required_argument, 0, 'b'},
        {"help",     no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "p:b:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p':
                port = optarg;
                break;
            case 'b':
                baudrate = atoi(optarg);
                if (baudrate <= 0) {
                    fprintf(stderr, "Erreur: vitesse invalide\n");
                    return 1;
                }
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    printf("=== Arduino Serial Monitor ===\n");
    printf("Port: %s\n", port);
    printf("Baud rate: %d\n", baudrate);
    printf("Type your commands (Ctrl+C to quit)\n");
    printf("==============================\n\n");
    
    if (configure_serial(port, baudrate) < 0) {
        return 1;
    }
    
    // Create the reading thread
    pthread_t tid;
    if (pthread_create(&tid, NULL, read_thread, NULL) != 0) {
        perror("Error creating thread");
        close(serial_fd);
        return 1;
    }
    
    // Main loop for sending commands
    char command[BUFFER_SIZE];
    while (running) {
        if (fgets(command, sizeof(command), stdin) != NULL) {
            int len = strlen(command);
            if (write(serial_fd, command, len) < 0) {
                perror("Error writing");
                break;
            }
        }
    }
    
    // Cleanup
    running = 0;
    pthread_join(tid, NULL);
    close(serial_fd);
    
    printf("\nMonitor closed.\n");
    return 0;
}