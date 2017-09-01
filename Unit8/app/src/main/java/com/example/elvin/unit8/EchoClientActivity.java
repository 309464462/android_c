package com.example.elvin.unit8;

import android.os.Bundle;
import android.widget.EditText;

import butterknife.BindView;

/**
 * Created by elvin on 2017/9/1.
 */

public class EchoClientActivity extends AbstractEchoActivity {


    @BindView(R.id.ip_edit)
    EditText ipEdit;
    @BindView(R.id.message_edit)
    EditText messageEdit;

    /**
     * Constructor.
     */
    public EchoClientActivity() {
        super(R.layout.activity_client);
    }

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    protected void onStartButtonClicked() {
        String ip = ipEdit.getText().toString();
        Integer port = getPort();
        String message = messageEdit.getText().toString();

        if ((0 != ip.length()) && (port != null) && (0 != message.length())) {
            ClientTask clientTask = new ClientTask(ip, port, message);
            clientTask.start();
        }
    }

    /**
     * Starts the TCP client with the given server IP address and port number,
     * and sends the given message.
     *
     * @param ip      IP address.
     * @param port    port number.
     * @param message message text.
     * @throws Exception
     */
    private native void nativeStartTcpClient(String ip, int port, String message)
            throws Exception;

    /**
     * Starts the UDP client with the given server IP address and port number.
     *
     * @param ip      IP address.
     * @param port    port number.
     * @param message message text.
     * @throws Exception
     */
    private native void nativeStartUdpClient(String ip, int port, String message)
            throws Exception;

    /**
     * Client task.
     */
    private class ClientTask extends AbstractEchoTask {
        /**
         * IP address to connect.
         */
        private final String ip;

        /**
         * Port number.
         */
        private final int port;

        /**
         * Message text to send.
         */
        private final String message;

        /**
         * Constructor.
         *
         * @param ip      IP address to connect.
         * @param port    port number to connect.
         * @param message message text to send.
         */
        public ClientTask(String ip, int port, String message) {
            this.ip = ip;
            this.port = port;
            this.message = message;
        }

        protected void onBackground() {
            logMessage("Starting client.");

            try {
                 nativeStartTcpClient(ip, port, message);
//                nativeStartUdpClient(ip, port, message);
            } catch (Throwable e) {
                logMessage(e.getMessage());
            }

            logMessage("Client terminated.");
        }
    }
}
