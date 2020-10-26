package com.example.toycarcontroller;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    View ball;
    View plate;
    TextView ballLocation;

    int x_axis;
    int y_axis;
    final static int MAX_LOCATION_VALUE = 255;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ball = findViewById(R.id.ball);
        plate = findViewById(R.id.plate);
        ballLocation = findViewById(R.id.ballLocation);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        ballLocation.setText("location: 0, 0");
        final int halfPlateWidth = plate.getWidth() / 2;
        final int halfBallWidth = ball.getWidth() / 2;
        final int halfPlateHeight = plate.getHeight() / 2;
        final int halfBallHeight = ball.getHeight() / 2;
        final int startRawLeft = halfPlateWidth - halfBallWidth;
        final int startRawTop = halfPlateHeight - halfBallHeight;
        final int startRawRight = halfPlateWidth + halfBallWidth;
        ;
        final int startRawBottom = halfPlateHeight + halfBallHeight;
        View.OnTouchListener handleTouch = new View.OnTouchListener() {
            private int startX;
            private int startY;

            @Override
            public boolean onTouch(View v, MotionEvent event) {

                int x = (int) event.getX();
                int y = (int) event.getY();

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        startX = x;
                        startY = y;
                        Log.i("TAG", "touched down");
                        break;
                    case MotionEvent.ACTION_MOVE:
                        int distanceX = x - startX;
                        int distanceY = y - startY;
                        float x_axis_float = (float) ((halfPlateWidth - (v.getLeft() + halfBallWidth)) * (-1.0) / halfPlateWidth * MAX_LOCATION_VALUE);
                        x_axis_float = x_axis_float >= MAX_LOCATION_VALUE ? MAX_LOCATION_VALUE : x_axis_float <= -MAX_LOCATION_VALUE ? -MAX_LOCATION_VALUE : x_axis_float;
                        float y_axis_float = halfPlateHeight - (v.getTop() + halfBallHeight);
                        y_axis_float = y_axis_float >= halfPlateWidth ? MAX_LOCATION_VALUE : y_axis_float <= halfPlateWidth * (-1) ? -MAX_LOCATION_VALUE : y_axis_float / halfPlateWidth * MAX_LOCATION_VALUE;
                        x_axis = (int) x_axis_float;
                        y_axis = (int) y_axis_float;
                        v.layout(
                                v.getLeft() + distanceX,
                                v.getTop() + distanceY,
                                v.getRight() + distanceX,
                                v.getBottom() + distanceY
                        );
                        Log.i("TAG", "x_axis y_axis: (" + x_axis + ", " + y_axis + ")");
                        Log.i("TAG", "moving: (" + x + ", " + y + ")");
                        ballLocation.setText("location: " + x_axis + ", " + y_axis);
                        new sendTask().execute(x_axis + "," + y_axis);
                        break;
                    case MotionEvent.ACTION_UP:
                        v.layout(
                                startRawLeft,
                                startRawTop,
                                startRawRight,
                                startRawBottom
                        );
                        ballLocation.setText("location: 0, 0");
                        new sendTask().execute("0,0");
                        Log.i("TAG", "touched up");
                        break;
                }
                return true;
            }
        };
        if (hasFocus) {
            ball.setOnTouchListener(handleTouch);
        }
    }

    public class sendTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... strings) {
            String uploadText = strings[0];
            someFunction.socketCommunication(uploadText);
            return "OK";
        }

    }

}


//keytool -genkey -v -keystore myApp.keystore -alias myApp.keystore -keyalg RSA -validity 30000
//password: 12345678