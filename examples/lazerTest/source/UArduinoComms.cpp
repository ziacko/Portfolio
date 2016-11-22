// Fill out your copyright notice in the Description page of Project Settings.

//#include "AceTroids_4_10.h"
#include "UArduinoComms.h"

#include <stdio.h>    // Standard input/output definitions
//#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
//#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
//#include <sys/ioctl.h>

int32 UUArduinoComms::initSerial(FString portName, int32 baudRate)
{
    return serialport_init(portName, baudRate);
}

FString UUArduinoComms::readData(int32 fd)
{
    serialport_read_until(fd, m_buf, '\r', sizeof m_buf, 100);
    return FString(ANSI_TO_TCHAR(m_buf));
}

FSplutData UUArduinoComms::readSplut(int32 fd)
{
    return serialport_read_splut_if_any(fd);
}

int32 UUArduinoComms::closeSerial(int32 fd)
{
    return serialport_close(fd);
}

//  Private methods
int32 UUArduinoComms::serialport_init(FString serialport, int32 baud)
{
    struct termios toptions;
    int32 fd;
    
    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(TCHAR_TO_ANSI(*serialport), O_RDWR | O_NONBLOCK );
    //fd = open("", O_RDWR | O_NONBLOCK );
    
    if (fd == -1)  {
        perror("serialport_init: Unable to open port ");
        return -1;
    }
    
    //int iflags = TIOCM_DTR;
    //ioctl(fd, TIOCMBIS, &iflags);     // turn on DTR
    //ioctl(fd, TIOCMBIC, &iflags);    // turn off DTR
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("serialport_init: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
        case 4800:   brate=B4800;   break;
        case 9600:   brate=B9600;   break;
#ifdef B14400
        case 14400:  brate=B14400;  break;
#endif
        case 19200:  brate=B19200;  break;
#ifdef B28800
        case 28800:  brate=B28800;  break;
#endif
        case 38400:  brate=B38400;  break;
        case 57600:  brate=B57600;  break;
        case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    
    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;
    
    //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset
    
    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    //toptions.c_cc[VTIME] = 20;
    
    tcsetattr(fd, TCSANOW, &toptions);
    if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
    
    return fd;
}

int32 UUArduinoComms::serialport_close( int32 fd )
{
    return close(fd);
}

int32 UUArduinoComms::serialport_read_until(int32 fd, char* buf, char until, int32 buf_max, int32 timeout)
{
    char b[1];  // read expects an array, so we give it a 1-byte array
    int i=0;
    do {
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 1 * 1000 );  // wait 1 msec try again
            timeout--;
            if( timeout==0 ) return -2;
            continue;
        }
#ifdef SERIALPORTDEBUG
        printf("serialport_read_until: i=%d, n=%d b='%c'\n",i,n,b[0]); // debug
#endif
        buf[i] = b[0];
        i++;
    } while( b[0] != until && i < buf_max && timeout>0 );
    
    buf[i] = 0;  // null terminate the string
    return 0;
}
FSplutData UUArduinoComms::serialport_read_splut_if_any(int32 fd)
{
    FSplutData data;
    
    data.splutReceived = false;
    
    data.xPos = 0;
    data.yPos = 0;
    data.ballSpeed = 0;
    data.ballSize = 0;
    
    int bytesInBuffer;
    ioctl(fd, FIONREAD, &bytesInBuffer);
    
    if (bytesInBuffer >= 4)
    {
        char b[4];
        int n = read(fd, b, 4);
        data.splutReceived = true;
        data.xPos       = ((unsigned char)b[0])/255.0f;
        data.yPos       = ((unsigned char)b[1])/255.0f;
        data.ballSpeed  = ((unsigned char)b[2])/255.0f;
        data.ballSize   = ((unsigned char)b[3])/255.0f;
    }
    
    return data;
}

TArray<FString> UUArduinoComms::getSerialPorts()
{
    TArray<FString> serialPortNames;
    
    FILE *fp;
    int status;
    char path[PATH_MAX];
    
    
    fp = popen("ls /dev/cu.*", "r");
    if (fp != NULL)
    {
        while (fgets(path, PATH_MAX, fp) != NULL)
            serialPortNames.Add(path);
    }
    status = pclose(fp);

    
    return serialPortNames;
}

FString UUArduinoComms::getSavedPath()
{
    return FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir());
}