package com.appsfromholland.esp8266;

import android.os.AsyncTask;
import android.provider.Settings;
import android.util.Log;

import org.json.JSONException;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * Created by dkroeske on 19/05/2017.
 */

public class PostESP8266Task extends AsyncTask<String, Void, String> {

    private boolean ledState = false;

    public PostESP8266Task(){
    }

    public void setLed(boolean ledState) {
        this.ledState = ledState;
    }

    @Override
    protected String doInBackground(String... strings) {

        String response = "";

        try {

            URL url = new URL(strings[0]);

            HttpURLConnection connection = (HttpURLConnection)url.openConnection();
            connection.setRequestMethod("POST");
            connection.setRequestProperty("USER-AGENT", "MijnBrowser/4.0");
            connection.setRequestProperty("Content-Type", "application/json");
            connection.setDoOutput(true);

            String body;
            if(ledState) {
                body = "{\"ledState\":true}";
            } else {
                body = "{\"ledState\":false}";
            }

            OutputStream os = connection.getOutputStream();
            BufferedWriter writer = new BufferedWriter( new OutputStreamWriter(os, "UTF-8"));
            writer.write(body);

            writer.flush();
            writer.close();
            os.close();

            int responseCode = connection.getResponseCode();

            if (responseCode == HttpURLConnection.HTTP_OK) {
                String line;
                BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                while( (line = reader.readLine()) != null ) {
                    response += line;
                }
            } else {
                response = "";
            }

            Log.i("", "responseCode : " + responseCode);

        } catch (MalformedURLException e) {
            Log.e("TAG", e.getLocalizedMessage());
        } catch (IOException e) {
            Log.e("TAG", e.getLocalizedMessage());
        } catch ( Exception e) {
            Log.e("TAG", e.getLocalizedMessage());
        }

        return response;

    }

    @Override
    protected void onPostExecute(String temp) {

    }
}
