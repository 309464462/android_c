#include "com_example_elvin_unit8_MainActivity.h"
#include "com_example_elvin_unit8_EchoClientActivity.h"
// JNI
#include <jni.h>

// NULL
#include <stdio.h>

// va_list, vsnprintf
#include <stdarg.h>

// errno
#include <errno.h>

// strerror_r, memset
#include <string.h>

// socket, bind, getsockname, listen, accept, recv, send, connect
#include <sys/types.h>
#include <sys/socket.h>

// sockaddr_un
#include <sys/un.h>

// htons, sockaddr_in
#include <netinet/in.h>

// inet_ntop
#include <arpa/inet.h>

// close, unlink
#include <unistd.h>

// offsetof
#include <stddef.h>

// Max log message length
#define MAX_LOG_MESSAGE_LENGTH 256

// Max data buffer size
#define MAX_BUFFER_SIZE 80

static void LogMessage(
        JNIEnv* env,
        jobject obj,
        const char* format,
        ...)
{
    // Cached log method ID
    static jmethodID methodID = NULL;

    // If method ID is not cached
    if (NULL == methodID)
    {
        // Get class from object
        jclass clazz = env->GetObjectClass(obj);

        // Get the method ID for the given method
        methodID = env->GetMethodID(clazz, "logMessage",
                                    "(Ljava/lang/String;)V");

        // Release the class reference
        env->DeleteLocalRef(clazz);
    }

    // If method is found
    if (NULL != methodID)
    {
        // Format the log message
        char buffer[MAX_LOG_MESSAGE_LENGTH];

        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        // Convert the buffer to a Java string
        jstring message = env->NewStringUTF(buffer);

        // If string is properly constructed
        if (NULL != message)
        {
            // Log message
            env->CallVoidMethod(obj, methodID, message);

            // Release the message reference
            env->DeleteLocalRef(message);
        }
    }
}




static void ThrowException(
        JNIEnv* env,
        const char* className,
        const char* message)
{
    // Get the exception class
    jclass clazz = env->FindClass(className);

    // If exception class is found
    if (NULL != clazz)
    {
        // Throw exception
        env->ThrowNew(clazz, message);

        // Release local class reference
        env->DeleteLocalRef(clazz);
    }
}

static void ThrowErrnoException(
        JNIEnv* env,
        const char* className,
        int errnum)
{
    char buffer[MAX_LOG_MESSAGE_LENGTH];

    // Get message for the error number
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH))
    {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
    }

    // Throw exception
    ThrowException(env, className, buffer);
}

static int NewTcpSocket(JNIEnv* env, jobject obj)
{
    // Construct socket
    LogMessage(env, obj, "Constructing a new TCP socket...");
    //PF_INET ipv4通信协议族 SOCK_STREAM tcp通信语义 0 为默认
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);

    // Check if socket is properly constructed
    if (-1 == tcpSocket)
    {
        // Throw an exception with error number
        //酷啊，这样的写法
        ThrowErrnoException(env, "java/io/IOException", errno);
    }

    return tcpSocket;
}

static void BindSocketToPort(
        JNIEnv* env,
        jobject obj,
        int sd,
        unsigned short port)
{
    struct sockaddr_in address;

    // Address to bind socket
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    // Bind to all addresses
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Convert port to network byte order
    address.sin_port = htons(port);

    // Bind socket
    LogMessage(env, obj, "Binding to port %hu.", port);
    //与一个地址绑定   sd 绑定到指定地址的socket实例
    if (-1 == bind(sd, (struct sockaddr*) &address, sizeof(address)))
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}

/**
 * Gets the port number socket is currently binded.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @return port number.
 * @throws IOException
 */
static unsigned short GetSocketPort(
        JNIEnv* env,
        jobject obj,
        int sd)
{
    unsigned short port = 0;

    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // Get the socket address
    if (-1 == getsockname(sd, (struct sockaddr*) &address, &addressLength))
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // Convert port to host byte order
        port = ntohs(address.sin_port);
        LogMessage(env, obj, "Binded to random port %hu.", port);
    }

    return port;
}

static void ListenOnSocket(
        JNIEnv* env,
        jobject obj,
        int sd,
        int backlog)
{
    // Listen on socket with the given backlog
    LogMessage(env, obj,"Listening on socket with a backlog of %d pending connections.",backlog);

    if (-1 == listen(sd, backlog))
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}


static void LogAddress(
        JNIEnv* env,
        jobject obj,
        const char* message,
        const struct sockaddr_in* address)
{
    char ip[INET_ADDRSTRLEN];

    // Convert the IP address to string
    if (NULL == inet_ntop(PF_INET,
                          &(address->sin_addr),
                          ip,
                          INET_ADDRSTRLEN))
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // Convert port to host byte order
        unsigned short port = ntohs(address->sin_port);

        // Log address
        LogMessage(env, obj, "%s %s:%hu.", message, ip, port);
    }
}

/**
 * Blocks and waits for incoming client connections on the
 * given socket.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @return client socket.
 * @throws IOException
 */
static int AcceptOnSocket(
        JNIEnv* env,
        jobject obj,
        int sd)
{
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // Blocks and waits for an incoming client connection
    // and accepts it
    LogMessage(env, obj, "Waiting for a client connection...");

    int clientSocket = accept(sd,
                              (struct sockaddr*) &address,
                              &addressLength);

    // If client socket is not valid
    if (-1 == clientSocket)
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // Log address
        LogAddress(env, obj, "Client connection from ", &address);
    }

    return clientSocket;
}


/**
 * Block and receive data from the socket into the buffer.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param buffer data buffer.
 * @param bufferSize buffer size.
 * @return receive size.
 * @throws IOException
 */
static ssize_t ReceiveFromSocket(
        JNIEnv* env,
        jobject obj,
        int sd,
        char* buffer,
        size_t bufferSize)
{
    // Block and receive data from the socket into the buffer
    LogMessage(env, obj, "Receiving from the socket...");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);

    // If receive is failed
    if (-1 == recvSize)
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // NULL terminate the buffer to make it a string
        buffer[recvSize] = NULL;

        // If data is received
        if (recvSize > 0)
        {
            LogMessage(env, obj, "Received %d bytes: %s", recvSize, buffer);
        }
        else
        {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return recvSize;
}

/**
 * Send data buffer to the socket.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param buffer data buffer.
 * @param bufferSize buffer size.
 * @return sent size.
 * @throws IOException
 */
static ssize_t SendToSocket(
        JNIEnv* env,
        jobject obj,
        int sd,
        const char* buffer,
        size_t bufferSize)
{
    // Send data buffer to the socket
    LogMessage(env, obj, "Sending to the socket...");
    ssize_t sentSize = send(sd, buffer, bufferSize, 0);

    // If send is failed
    if (-1 == sentSize)
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        if (sentSize > 0)
        {
            LogMessage(env, obj, "Sent %d bytes: %s", sentSize, buffer);
        }
        else
        {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return sentSize;
}


void Java_com_example_elvino_unit8_MainActivity_nativeStartTcpServer(
        JNIEnv* env,
        jobject obj,
        jint port)
{
    // Construct a new TCP socket.
    int serverSocket = NewTcpSocket(env, obj);
    if (NULL == env->ExceptionOccurred())
    {
        // Bind socket to a port number
        BindSocketToPort(env, obj, serverSocket, (unsigned short) port);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // If random port number is requested
        if (0 == port)
        {
            // Get the port number socket is currently binded
            GetSocketPort(env, obj, serverSocket);
            if (NULL != env->ExceptionOccurred())
                goto exit;
        }

        // Listen on socket with a backlog of 4 pending connections
        ListenOnSocket(env, obj, serverSocket, 4);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // Accept a client connection on socket
        int clientSocket = AcceptOnSocket(env, obj, serverSocket);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sentSize;

        // Receive and send back the data
        while (1)
        {
            // Receive from the socket
            recvSize = ReceiveFromSocket(env, obj, clientSocket,
                                         buffer, MAX_BUFFER_SIZE);

            if ((0 == recvSize) || (NULL != env->ExceptionOccurred()))
                break;

            // Send to the socket
            sentSize = SendToSocket(env, obj, clientSocket,
                                    buffer, (size_t) recvSize);

            if ((0 == sentSize) || (NULL != env->ExceptionOccurred()))
                break;
        }

        // Close the client socket
        close(clientSocket);
    }

    exit:
    if (serverSocket > 0)
    {
        close(serverSocket);
    }
}


/**
 * Connects to given IP address and given port.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param ip IP address.
 * @param port port number.
 * @throws IOException
 */
static void ConnectToAddress(
        JNIEnv* env,
        jobject obj,
        int sd,
        const char* ip,
        unsigned short port)
{
    // Connecting to given IP address and given port
    LogMessage(env, obj, "Connecting to %s:%uh...", ip, port);

    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    // Convert IP address string to Internet address
    if (0 == inet_aton(ip, &(address.sin_addr)))
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // Convert port to network byte order
        address.sin_port = htons(port);

        // Connect to address
        if (-1 == connect(sd, (const sockaddr*) &address, sizeof(address)))
        {
            // Throw an exception with error number
            ThrowErrnoException(env, "java/io/IOException", errno);
        }
        else
        {
            LogMessage(env, obj, "Connected.");
        }
    }
}


void Java_com_example_elvino_unit8_EchoClientActivity_nativeStartTcpClient(
        JNIEnv* env,
        jobject obj,
        jstring ip,
        jint port,
        jstring message)
{
    // Construct a new TCP socket.
    int clientSocket = NewTcpSocket(env, obj);
    if (NULL == env->ExceptionOccurred())
    {
        // Get IP address as C string
        const char* ipAddress = env->GetStringUTFChars(ip, NULL);
        if (NULL == ipAddress)
            goto exit;

        // Connect to IP address and port
        ConnectToAddress(env, obj, clientSocket, ipAddress,
                         (unsigned short) port);

        // Release the IP address
        env->ReleaseStringUTFChars(ip, ipAddress);

        // If connection was successful
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // Get message as C string
        const char* messageText = env->GetStringUTFChars(message, NULL);
        if (NULL == messageText)
            goto exit;

        // Get the message size
        jsize messageSize = env->GetStringUTFLength(message);

        // Send message to socket
        SendToSocket(env, obj, clientSocket, messageText, messageSize);

        // Release the message text
        env->ReleaseStringUTFChars(message, messageText);

        // If send was not successful
        if (NULL != env->ExceptionOccurred())
            goto exit;

        char buffer[MAX_BUFFER_SIZE];

        // Receive from the socket
        ReceiveFromSocket(env, obj, clientSocket, buffer, MAX_BUFFER_SIZE);
    }

    exit:
    if (clientSocket > 0)
    {
        close(clientSocket);
    }
}
