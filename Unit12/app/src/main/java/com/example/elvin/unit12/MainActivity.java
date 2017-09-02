package com.example.elvin.unit12;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AbstractPlayerActivity {

    @BindView(R.id.file_name_edit)
    EditText fileNameEdit;
    @BindView(R.id.bitmap_player_radio)
    RadioButton bitmapPlayerRadio;
    @BindView(R.id.player_radio_group)
    RadioGroup playerRadioGroup;
    @BindView(R.id.play_button)
    Button playButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.play_button)
    public void OnClick(View view){
        switch (view.getId()){
            case R.id.play_button:
                onPlayButtonClick();
                break;
            default:
                break;
        }
    }


    /**
     * On play button click event handler.
     */
    private void onPlayButtonClick() {
        Intent intent;

        // Get the checked radio button id
        int radioId = playerRadioGroup.getCheckedRadioButtonId();

        // Choose the activity based on id
        switch (radioId) {
            case R.id.bitmap_player_radio:
                intent = new Intent(this, BitmapPlayerActivity.class);
                break;

            default:
                throw new UnsupportedOperationException("radioId=" + radioId);
        }

        // Under the external storage
        File file = new File(Environment.getExternalStorageDirectory(),
                fileNameEdit.getText().toString());

        // Put AVI file name as extra
        intent.putExtra(AbstractPlayerActivity.EXTRA_FILE_NAME,
                file.getAbsolutePath());

        // Start the player activity
        startActivity(intent);
    }
}
