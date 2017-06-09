package com.appsfromholland.esp8266;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity{

    private String[] urls = new String[] {"http://172.20.10.3:8080"};
    private TextView tv;
    private Switch autoUpdateSwitch;
    private Handler handler = new Handler();


    private Runnable runnableCode = new Runnable() {
        @Override
        public void run() {
            Log.d("", "Aangeroepen op de MAIN (GUI) thread. Dat is goed");
            updateCounterValue();

            handler.postDelayed(runnableCode, 500);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        //
        tv = (TextView) findViewById(R.id.esp_counter_id);

        //
        autoUpdateSwitch = (Switch) findViewById(R.id.auto_update_id);

        findViewById(R.id.update_bnt_id).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new ESP8266Task(new ESP8266Listener() {
                    @Override
                    public void onESP8266CounterAvailable(int counterValue) {
                        tv.setText(String.valueOf(counterValue));
                    }

                    public void onESP8266Error() {
                        tv.setText("ESP NOT READY");
                    }
                }).execute(urls);
            }
        });


        // Led On
        findViewById(R.id.ledon_id).setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                PostESP8266Task task = new PostESP8266Task();
                task.setLed(true);
                task.execute(urls);
            }
        });

        // Led Off
        findViewById(R.id.ledoff_id).setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                PostESP8266Task task = new PostESP8266Task();
                task.setLed(false);
                task.execute(urls);
            }
        });

        //
        handler.post(runnableCode);
    }

    //
    //
    //
    private void updateCounterValue() {
        if( autoUpdateSwitch.isChecked() ) {
            new ESP8266Task(new ESP8266Listener() {
                @Override
                public void onESP8266CounterAvailable(int counterValue) {
                    tv.setText(String.valueOf(counterValue));
                }

                public void onESP8266Error() {
                    tv.setText("ESP NOT READY");
                }
            }).execute(urls);
        }
    }

}
