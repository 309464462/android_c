package com.example.elvin.unit8;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.os.Bundle;
import android.widget.EditText;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;

import butterknife.BindView;

/**
 * 在原生层实现本地socket服务器
 * 在java曾实现本地客户端
 * 在两个应用程序之间本地socket通信
 * Created by elvin on 2017/9/2.
 */

public class LocalEchoActivity extends AbstractEchoActivity {

    @BindView(R.id.message_edit)
    EditText messageEdit;

    public LocalEchoActivity() {
        super(R.layout.activity_echo);
    }

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    protected void onStartButtonClicked() {
        String name = etPort.getText().toString();
        String message = messageEdit.getText().toString();

        if ((name.length() > 0) && (message.length() > 0)) {
            String socketName;

            // If it is a filesystem socket, prepend the
            // application files directory
            if (isFilesystemSocket(name)) {
                File file = new File(getFilesDir(), name);
                socketName = file.getAbsolutePath();
            } else {
                socketName = name;
            }

            ServerTask serverTask = new ServerTask(socketName);
            serverTask.start();

            ClientTask clientTask = new ClientTask(socketName, message);
            clientTask.start();
        }
    }

    /**
     * Check if name is a filesystem socket.
     *
     * @param name socket name.
     * @return filesystem socket.
     */
    private boolean isFilesystemSocket(String name) {
        return name.startsWith("/");
    }

    /**
     * Starts the Local UNIX socket server binded to given name.
     *
     * @param name socket name.
     * @throws Exception
     */
    private native void nativeStartLocalServer(String name) throws Exception;

    /**
     * Starts the local UNIX socket client.
     *
     * @param name    port number.
     * @param message message text.
     * @throws Exception
     */
    private void startLocalClient(String name, String message) throws Exception {
        // 创建一个localsocket
        LocalSocket clientSocket = new LocalSocket();
        try {
            // 设置本地socket的名字域
            LocalSocketAddress.Namespace namespace;
            //判断是否为文件路径
            if (isFilesystemSocket(name)) {
                namespace = LocalSocketAddress.Namespace.FILESYSTEM;
            } else {
                namespace = LocalSocketAddress.Namespace.ABSTRACT;
            }

            // Construct local socket address
            LocalSocketAddress address = new LocalSocketAddress(name, namespace);

            // Connect to local socket
            logMessage("Connecting to " + name);
            clientSocket.connect(address);
            logMessage("Connected.");

            // Get message as bytes
            byte[] messageBytes = message.getBytes();

            // Send message bytes to the socket
            logMessage("Sending to the socket...");
            OutputStream outputStream = clientSocket.getOutputStream();
            outputStream.write(messageBytes);
            logMessage(String.format("Sent %d bytes: %s",
                    messageBytes.length, message));

            // Receive the message back from the socket
            logMessage("Receiving from the socket...");
            InputStream inputStream = clientSocket.getInputStream();
            int readSize = inputStream.read(messageBytes);

            String receivedMessage = new String(messageBytes, 0, readSize); //这个用法很少见
            logMessage(String.format("Received %d bytes: %s",
                    readSize, receivedMessage));

            // Close streams
            outputStream.close();
            inputStream.close();

        } finally {
            // Close the local socket
            clientSocket.close();
        }
    }

    /**
     * Server task.
     */
    private class ServerTask extends AbstractEchoTask {
        /**
         * Socket name.
         */
        private final String name;

        /**
         * Constructor.
         *
         * @param name socket name.
         */
        public ServerTask(String name) {
            this.name = name;
        }

        protected void onBackground() {
            logMessage("Starting server.");

            try {
                nativeStartLocalServer(name);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Server terminated.");
        }
    }

    /**
     * Client task.
     */
    private class ClientTask extends Thread {
        /**
         * Socket name.
         */
        private final String name;

        /**
         * Message text to send.
         */
        private final String message;

        /**
         * Constructor.
         *
         * @param message message text to send.
         * @parma name socket name.
         */
        public ClientTask(String name, String message) {
            this.name = name;
            this.message = message;
        }

        public void run() {
            logMessage("Starting client.");

            try {
                startLocalClient(name, message);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Client terminated.");
        }
    }
}
