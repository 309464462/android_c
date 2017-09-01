package com.example.elvin.unit8;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * Created by elvin on 2017/9/1.
 */

public abstract class AbstractEchoActivity extends AppCompatActivity {

    @BindView(R.id.etPort)
    EditText etPort;
    @BindView(R.id.btnStart)
    Button btnStart;
    @BindView(R.id.tvlog)
    TextView tvlog;
    @BindView(R.id.svView)
    ScrollView svView;

    /**
     * Layout ID.
     */
    private int layoutID;

    public AbstractEchoActivity(int layoutID) {
        this.layoutID = layoutID;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutID);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.btnStart)
    public void OnClick(View view) {
        switch (view.getId()) {
            case R.id.btnStart:
                onStartButtonClicked();
                break;
            default:
                break;
        }
    }

    protected abstract void onStartButtonClicked();

    protected Integer getPort() {
        Integer port;

        try {
            port = Integer.valueOf(etPort.getText().toString());
        } catch (NumberFormatException e) {
            port = null;
        }

        return port;
    }

    protected void logMessage(final String message) {
        runOnUiThread(new Runnable() {
            public void run() {
                logMessageDirect(message);
            }
        });
    }

    protected void logMessageDirect(final String message) {
        tvlog.append(message);
        tvlog.append("\n");
        svView.fullScroll(View.FOCUS_DOWN);
    }

    protected abstract class AbstractEchoTask extends Thread {
        /** Handler object. */
        private final Handler handler;

        /**
         * Constructor.
         */
        public AbstractEchoTask() {
            handler = new Handler();
        }

        /**
         * On pre execute callback in calling thread.
         */
        protected void onPreExecute() {
            btnStart.setEnabled(false);
            tvlog.setText("");
        }

        public synchronized void start() {
            onPreExecute();
            super.start();
        }

        public void run() {
            onBackground();

            handler.post(new Runnable() {
                public void run() {
                    onPostExecute();
                }
            });
        }

        /**
         * On background callback in new thread.
         */
        protected abstract void onBackground();

        /**
         * On post execute callback in calling thread.
         */
        protected void onPostExecute() {
            btnStart.setEnabled(true);
        }
    }

    static {
        System.loadLibrary("Echo");
    }
}
