package com.example.elvin.unit8;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
/*
* 面向连接的socket通信实例
* */
public class MainActivity extends AbstractEchoActivity {

    public MainActivity() {
        super(R.layout.activity_main);
    }

    @Override
    protected void onStartButtonClicked() {
        Integer port = getPort();
        if (port != null) {
            ServerTask serverTask = new ServerTask(port);
            serverTask.start();
        }
    }


    private native void nativeStartTcpServer(int port) throws Exception;

    private native void nativeStartUdpServer(int port) throws Exception;

    private class ServerTask extends AbstractEchoTask {
        /** Port number. */
        private final int port;

        /**
         * Constructor.
         *
         * @param port
         *            port number.
         */
        public ServerTask(int port) {
            this.port = port;
        }

        protected void onBackground() {
            logMessage("Starting server.");

            try {
//                 nativeStartTcpServer(port);
                nativeStartUdpServer(port);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Server terminated.");
        }
    }
}
