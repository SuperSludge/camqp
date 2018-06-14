package com.example.camqp.camqp;

import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        final Button button = findViewById(R.id.button_id);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.e("START OPEN", "OPEN");
                button.setText("O P E N I N G");
                button.setBackgroundColor(Color.GREEN);
                open();
                    Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        public void run() {
                            button.setBackgroundColor(Color.GRAY);
                            button.setText("open");
                        }
                    }, 2000);

            }
        });
    }





    public  void open()
    {
        new PublishMessageTask().execute("foo");
    }





}

class PublishMessageTask extends AsyncTask<String, Void, Long> {

    private Exception exception;
    ConnectionFactory factory = new ConnectionFactory();

    protected Long doInBackground(String... message) {
        long retval = -1;
        try {

            Log.e("publish", "start");

            String uri = "url";
            factory.setUri(uri);

            factory.setRequestedHeartbeat(30);
            factory.setConnectionTimeout(10000);

            Log.e("publish", "factory init");

            Connection connection = factory.newConnection();
            Log.e("publish", "connection");

            Channel channel = connection.createChannel();


            channel.basicPublish("","doordings",null , ("foo").getBytes());
            channel.basicPublish("","yellow",null , ("foo").getBytes());

            Log.e("publish", "published!");


            retval = 0;
        } catch (Exception e) {
            this.exception = e;

        }

        return retval;
    }

    protected void onPostExecute(Long feed) {
        if(exception != null) {
            Log.e("PUBLISH FAIL", exception.getMessage());
        }
    }
}
