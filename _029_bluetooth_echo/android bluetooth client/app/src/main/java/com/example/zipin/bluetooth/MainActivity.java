package com.example.zipin.bluetooth;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = "bluetooth1";

    Button btnOn, btnOff;

    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;
    private TextView txtResponse=null;

    // SPP UUID service. server and client must have same UUID. default uuid is 00001101-0000-1000-8000-00805F9B34FB
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //"fa87c0d0-afac-11de-8a39-0800200c9a66");
    //"00001101-0000-1000-8000-00805F9B34FB"); default uuid


    // MAC-address of Bluetooth module
    //device name of bluetooth (you must edit this line)
    private static String address = ""; //"78:0C:B8:3F:AC:5E";
    private static String deviceName="HC-06";


    private long mLastClickTime = 0;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        btnOn = (Button) findViewById(R.id.btnOn);
        btnOff = (Button) findViewById(R.id.btnOff);
        txtResponse=(TextView)findViewById(R.id.txtResponse);

        btAdapter = BluetoothAdapter.getDefaultAdapter();

        checkBTState();

        GetPairedDevice();

        btnOn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();


                sendData("123\n\r");
               // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });

        btnOff.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 500){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();



                sendData("01234\n\r");
               // Toast.makeText(getBaseContext(), "Turn off LED", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        if(Build.VERSION.SDK_INT >= 10){
            try {
                final Method  m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", new Class[] { UUID.class });
                return (BluetoothSocket) m.invoke(device, MY_UUID);
            } catch (Exception e) {
                Log.e(TAG, "Could not create Insecure RFComm Connection",e);
            }
        }
        return  device.createRfcommSocketToServiceRecord(MY_UUID);
    }

    @Override
    public void onResume() {
        super.onResume();
/*
        Log.d(TAG, "...onResume - try connect...");

        // Set up a pointer to the remote node using it's address.
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        // Two things are needed to make a connection:
        //   A MAC address, which we got above.
        //   A Service ID or UUID.  In this case we are using the
        //     UUID for SPP.

        try {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e1) {
            errorExit("Fatal Error", "In onResume() and socket create failed: " + e1.getMessage() + ".");
        }

    /*try {
      btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
    } catch (IOException e) {
      errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
    }*/
/*
        // Discovery is resource intensive.  Make sure it isn't going on
        // when you attempt to connect and pass your message.
        btAdapter.cancelDiscovery();

        // Establish the connection.  This will block until it connects.
        Log.d(TAG, "...Connecting...");
        try {
            btSocket.connect();
            Log.d(TAG, "...Connection ok...");
            outStream = btSocket.getOutputStream();
            outStream.write(65);
            outStream.flush();
            outStream.close();
            btSocket.close();
            Log.d(TAG, "...Connection closed ok...");
        } catch (IOException e) {
            try {
                btSocket.close();
            } catch (IOException e2) {
                errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
            }
        }

        // Create a data stream so we can talk to server.
        //Log.d(TAG, "...Create Socket...");

       // try {
        //    outStream = btSocket.getOutputStream();
        //} catch (IOException e) {
        //    errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");
        //}
        */
    }

    @Override
    public void onPause() {
        super.onPause();

        /*
        Log.d(TAG, "...In onPause()...");

        if (outStream != null) {
            try {
                outStream.flush();
            } catch (IOException e) {
                errorExit("Fatal Error", "In onPause() and failed to flush output stream: " + e.getMessage() + ".");
            }
        }

        try     {
            btSocket.close();
        } catch (IOException e2) {
            errorExit("Fatal Error", "In onPause() and failed to close socket." + e2.getMessage() + ".");
        }
        */
    }

    private void checkBTState() {
        // Check for Bluetooth support and then check to make sure it is turned on
        // Emulator doesn't support Bluetooth and will return null
        if(btAdapter==null) {
            errorExit("Fatal Error", "Bluetooth not support");
        } else {
            if (btAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth ON...");
            } else {
                //Prompt user to turn on Bluetooth
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }

    private  void GetPairedDevice()
    {
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                // Add the name and address to an array adapter to show in a ListView
               // mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                if(device.getName().equals(deviceName))
                {
                    address=device.getAddress();
                }
                Log.i("bluetooth devices", device.getName() + "\n" + device.getAddress());
            }
        }
    }

    private void errorExit(String title, String message){
        Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_LONG).show();
        finish();
    }

    static boolean _busy=false;
    private void sendData(String message) {
        btnOn.setEnabled(false);
        btnOff.setEnabled(false);
        btnOn.setClickable(false);
        btnOff.setClickable(false);

        if (_busy==true)
            return;

        _busy=true;

        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, "...Send data: " + message + "...");

        try {
            btAdapter.cancelDiscovery();
            BluetoothDevice device = btAdapter.getRemoteDevice(address);
            btSocket = createBluetoothSocket(device);
            btSocket.connect();
            outStream = btSocket.getOutputStream();
            outStream.write(msgBuffer);
            outStream.flush();

            byte[] buffer = new byte[256];
            InputStream inputStream= btSocket.getInputStream();

            //need sleep otherwise data sent from arduino will split in two pieces. e.g 'you have entered xxx', android receive, 'y' then the rest.
            //default bluetooth HC-06 baud rate is 9600, change it to 57600 use program _029_bluetooth_change_baudrate(maybe it help)
            //http://stackoverflow.com/questions/3342651/how-can-i-delay-a-java-program-for-a-few-seconds
            TimeUnit.MILLISECONDS.sleep(500);

            int bytes = inputStream.read(buffer,0, 256);
            //bytes = inputStream.read(buffer,0, 256);
            String readMessage = new String(buffer, 0, 256);
            inputStream.close();
            outStream.close();
            btSocket.close();
            txtResponse.setText(readMessage);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if (address.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 35 in the java code";
            msg = msg +  ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";

            errorExit("Fatal Error", msg);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        finally {
            _busy=false;
            btnOn.setEnabled(true);
            btnOff.setEnabled(true);
            btnOn.setClickable(true);
            btnOff.setClickable(true);

        }
    }
}

