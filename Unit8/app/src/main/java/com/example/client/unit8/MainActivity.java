package com.example.client.unit8;

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
*
* （1）知名端口（Well-Known Ports）
知名端口即众所周知的端口号，范围从0到1023，这些端口号一般固定分配给一些服务。
比如
21端口分配给FTP服务，
25端口分配给SMTP（简单邮件传输协议）服务，
80端口分配给HTTP服务，
135端口分配给RPC（远程过程调用）服务等等。

（2）动态端口（Dynamic Ports）
动态端口的范围从1024到65535，这些端口号一般不固定分配给某个服务，
也就是说许多服务都可以使用这些端口。只要运行的程序向系统提出访问网络的申请，
那么系统就可以从这些端口号中分配一个供该程序使用。
比如1024端口就是分配给第一个向系统发出申请的程序。
在关闭程序进程后，就会释放所占用的端口号。
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
                 nativeStartTcpServer(port);
//                nativeStartUdpServer(port);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Server terminated.");
        }
    }
}
