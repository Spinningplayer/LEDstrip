package com.appsfromholland.esp8266;

import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

/**
 * Created by dkroeske on 15/05/2017.
 */

public class ESP8266Task extends AsyncTask<String,Void,String> {

    // Call back
    private ESP8266Listener listener = null;

    // Constructor, set listener
    public ESP8266Task(ESP8266Listener listener) {
        this.listener = listener;
    }

    // Static's
    private static final String TAG = "ESP8266Task";

    @Override
    protected String doInBackground(String... params) {

        InputStream inputStream = null;
        BufferedReader reader = null;
        String urlString = "";

        String response = "";

        for (String url : params) {
            Log.i(TAG, url);
        }

        try {
            URL url = new URL(params[0]);
            URLConnection connection = url.openConnection();

            reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            response = reader.readLine().toString();
            String line;
            while ( (line = reader.readLine()) != null ) {
                response += line;
            }

        } catch (MalformedURLException e) {
            Log.e("TAG", e.getLocalizedMessage());
            return null;
        } catch (IOException e) {
            Log.e("TAG", e.getLocalizedMessage());
            return null;
        } catch ( Exception e) {
            Log.e("TAG", e.getLocalizedMessage());
            return null;
        } finally {
            if( reader != null ) {
                try {
                    reader.close();
                } catch (IOException e) {
                    Log.e("TAG", e.getLocalizedMessage());
                    return null;
                }
            }
        }

        return response;
    }

    protected void onProgressUpdate(Integer... progress) {
        Log.i(TAG, progress.toString());
    }

    protected void onPostExecute(String response) {

        if( response != null ) {
            try {
                JSONObject jsonObject = new JSONObject(response);
                int value = jsonObject.getInt("counter");
                listener.onESP8266CounterAvailable(value);
            } catch (JSONException ex) {
                Log.e(TAG, ex.toString());
                listener.onESP8266Error();
            }
        } else {
            listener.onESP8266Error();
        }
    }

}
