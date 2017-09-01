#include "com_example_elvin_unit8_MainActivity.h"

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

static int NewTcpSocket(JNIEnv* env, jobject obj)
{
    // Construct socket
    LogMessage(env, obj, "Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);

    // Check if socket is properly constructed
    if (-1 == tcpSocket)
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }

    return tcpSocket;
}

void Java_com_example_elvin_unit8_MainActivity_nativeStartTcpServer(
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