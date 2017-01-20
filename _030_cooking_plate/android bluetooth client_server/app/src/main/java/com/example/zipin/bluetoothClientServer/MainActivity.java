package com.example.zipin.bluetoothClientServer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.example.zipin.bluetoothClientServer.BluetoothChatService;

public class MainActivity extends Activity implements OnClickListener {
    private static final String TAG = "bluetooth1";

    Button btnSendCommand, btnConnect, btnOnOff, btnMode, btnPlus, btnMinus, btnMaximum, btnMinimum, btnSendCommandWithTemperatureAndTime, btnTemperature;
    Button btn10, btn15, btn20, btn25, btn30, btn35, btn40, btn45, btn50, btn55, btn60, btn65;
    TextView textViewConnectingStatus, textViewWorkingStatus;
    Spinner spinnerCommands;
    EditText editTextDesiredTemperature, editTextContinueTimeAfterDesiredTemperature;

    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;
    private InputStream inputStream=null;


    private static byte END_BYTE=(byte)127;

    // SPP UUID service. server and client must have same UUID. default uuid is 00001101-0000-1000-8000-00805F9B34FB
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //"fa87c0d0-afac-11de-8a39-0800200c9a66");
    //"00001101-0000-1000-8000-00805F9B34FB"); default uuid


    // MAC-address of Bluetooth module
    //device name of bluetooth (you must edit this line)
    private static String address = ""; //"78:0C:B8:3F:AC:5E";
    private static String deviceName="HC-06";


    private long mLastClickTime = 0;

    private BluetoothChatService mChatService = null;
    MediaPlayer _mediaPlayer=null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        btnSendCommand = (Button) findViewById(R.id.btnSendCommand);
        btnConnect = (Button) findViewById(R.id.btnConnect);

        btnOnOff = (Button) findViewById(R.id.btnOnOff);
        btnMode=(Button)findViewById(R.id.btnMode);
        btnPlus=(Button)findViewById(R.id.btnPlus);
        btnMinus=(Button)findViewById(R.id.btnMinus);
        btnMaximum=(Button)findViewById(R.id.btnMaximum);
        btnMinimum=(Button)findViewById(R.id.btnMinimum);
        btnSendCommandWithTemperatureAndTime=(Button)findViewById(R.id.btnSendCommandWithTemperatureAndTime);
        btnTemperature=(Button)findViewById(R.id.btnTemperature);
        btn10=(Button)findViewById(R.id.btn10);
        btn15=(Button)findViewById(R.id.btn15);
        btn20=(Button)findViewById(R.id.btn20);
        btn25=(Button)findViewById(R.id.btn25);
        btn30=(Button)findViewById(R.id.btn30);
        btn35=(Button)findViewById(R.id.btn35);
        btn40=(Button)findViewById(R.id.btn40);
        btn45=(Button)findViewById(R.id.btn45);
        btn50=(Button)findViewById(R.id.btn50);
        btn55=(Button)findViewById(R.id.btn55);
        btn60=(Button)findViewById(R.id.btn60);
        btn65=(Button)findViewById(R.id.btn65);

        btn10.setOnClickListener(this);
        btn15.setOnClickListener(this);
        btn20.setOnClickListener(this);
        btn25.setOnClickListener(this);
        btn30.setOnClickListener(this);;
        btn35.setOnClickListener(this);
        btn40.setOnClickListener(this);
        btn45.setOnClickListener(this);
        btn50.setOnClickListener(this);;
        btn55.setOnClickListener(this);
        btn60.setOnClickListener(this);
        btn65.setOnClickListener(this);

        textViewConnectingStatus=(TextView)findViewById(R.id.textViewConnectingStatus);
        textViewWorkingStatus=(TextView)findViewById(R.id.textViewWorkingStatus);

        editTextContinueTimeAfterDesiredTemperature=(EditText)findViewById(R.id.editTextContinueTimeAfterDesiredTemperature);
        editTextDesiredTemperature=(EditText)findViewById(R.id.editTextDesiredTemperature);

        spinnerCommands=(Spinner)findViewById(R.id.spinnerCommands);

        btAdapter = BluetoothAdapter.getDefaultAdapter();


       // _mediaPlayer= MediaPlayer.create(getApplicationContext(), R.raw.voice);
       // _mediaPlayer.();


        checkBTState();

        GetPairedDevice();

        btnSendCommand.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();


                if (spinnerCommands.getSelectedItem() == null)
                    return;
                //byte[] data = {'a', 1, END_BYTE}; //(byte)255 is -1

                // sendData(data);
                // if(edtCommandNumber.getText().toString().equals(""))
                //     return;;

                int commandNumber = (int) spinnerCommands.getSelectedItemId();
                byte[] data = {'a', (byte) commandNumber, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });


        btnConnect.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                /*
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 500){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();
                btAdapter.cancelDiscovery();
                BluetoothDevice device = btAdapter.getRemoteDevice(address);
                try {
                    btSocket = createBluetoothSocket(device);
                    btSocket.connect();
                    outStream = btSocket.getOutputStream();
                    inputStream= btSocket.getInputStream();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                */
                btAdapter.cancelDiscovery();
                BluetoothDevice device = btAdapter.getRemoteDevice(address);
                mChatService.connect(device, true);
                //mChatService.connected(device, true);
                setConnectionStatusText();
            }
        });
        btnOnOff.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 1, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });
        btnMode.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 2, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });
        btnPlus.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 3, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });
        btnMinus.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 4, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });
        btnMaximum.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 5, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });
        btnMinimum.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 6, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });

        btnSendCommandWithTemperatureAndTime.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                if(editTextDesiredTemperature.getText().toString().equals("") || editTextContinueTimeAfterDesiredTemperature.getText().toString().equals(""))
                    return;;

                byte[] data = {'d', (byte)Integer.parseInt(editTextDesiredTemperature.getText().toString()), (byte)Integer.parseInt(editTextContinueTimeAfterDesiredTemperature.getText().toString()), END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });

        btnTemperature.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 1000) {
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                byte[] data = {'b', 7, END_BYTE}; //(byte)255 is -1
                mChatService.write(data);
                // Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
            }
        });

        /*
        btnDisconnect.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                // mis-clicking prevention, using threshold of 1000 ms
                if (SystemClock.elapsedRealtime() - mLastClickTime < 500){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();

                if (mChatService != null) {
                    mChatService.stop();
                }
            }
        });
        */



        List<String> SpinnerArray =  new ArrayList<String>();
        try {
           // int selectedIndex=0;
            SpinnerArray.add("Cooking0");
            SpinnerArray.add("Cooking10");
            SpinnerArray.add("Cooking15");
            SpinnerArray.add("Cooking20");
            SpinnerArray.add("Cooking25");
            SpinnerArray.add("Cooking30");
            SpinnerArray.add("Cooking45");
            SpinnerArray.add("Cooking60");

            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, SpinnerArray);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            spinnerCommands.setAdapter(adapter);

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }




    @Override
    protected void onStart() {
        super.onStart();
        if (mChatService == null) {
            setupChat();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mChatService != null) {
            mChatService.stop();
           // setConnectionStatusText();
        }
    }

    private void setupChat() {
        Log.d(TAG, "setupChat()");

        // Initialize the BluetoothChatService to perform bluetooth connections
        mChatService = new BluetoothChatService(getApplicationContext(), mHandler);
        mChatService.start();
        setConnectionStatusText();
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

        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
                // Start the Bluetooth chat services
                mChatService.start();
            }
        }
        setConnectionStatusText();

        if(_mediaPlayer!=null)
            _mediaPlayer.release();

        _mediaPlayer= MediaPlayer.create(getApplicationContext(), R.raw.voice);

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

        if(_mediaPlayer!=null)
           _mediaPlayer.release();

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
        byte[] msgBuffer = message.getBytes();
        sendData(msgBuffer);
    }

    private void sendData(byte[] data) {
        btnSendCommand.setEnabled(false);
        btnSendCommand.setClickable(false);

        if (_busy == true)
            return;

        _busy = true;

        try {
            btAdapter.cancelDiscovery();
            BluetoothDevice device = btAdapter.getRemoteDevice(address);
            btSocket = createBluetoothSocket(device);
            btSocket.connect();
            outStream = btSocket.getOutputStream();
            outStream.write(data);
            outStream.flush();

            byte[] buffer = new byte[256];
            TimeUnit.MILLISECONDS.sleep(200);
            InputStream inputStream = btSocket.getInputStream();

            //need sleep otherwise data sent from arduino will split in two pieces. e.g 'you have entered xxx', android receive, 'y' then the rest.
            //default bluetooth HC-06 baud rate is 9600, change it to 57600 use program _029_bluetooth_change_baudrate(maybe it help)
            //http://stackoverflow.com/questions/3342651/how-can-i-delay-a-java-program-for-a-few-seconds
            //TimeUnit.MILLISECONDS.sleep(1000);
            /*
            int pos=0;
            int bytes = inputStream.read(buffer, pos, 256);
            while (bytes>0)
            {
                pos+=bytes;
                bytes = inputStream.read(buffer, pos, 256-pos);
            }
            */

            int pos=0;
            int bytes = inputStream.read(buffer, pos, 256);
            while (containsEndByte(buffer)==false)
            {
                pos+=bytes;
                bytes = inputStream.read(buffer, pos, 256-pos);
            }

            //bytes = inputStream.read(buffer,0, 256);
            String readMessage = new String(buffer, 0, 256);
            inputStream.close();
            outStream.close();
            btSocket.close();
            textViewWorkingStatus.setText(readMessage);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if (address.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 35 in the java code";
            msg = msg + ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";

            errorExit("Fatal Error", msg);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            _busy = false;
            btnSendCommand.setEnabled(true);
            btnSendCommand.setClickable(true);
        }
    }

    StringBuilder sb=new StringBuilder();
    byte[] bufTem= new byte[1024];
    int bufTempPos=0;

    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Constants.MESSAGE_READ:
                   // boolean hasEndByte=false;
                    byte[] readBuf = (byte[]) msg.obj;

                    for (int i=0; i<msg.arg1; i++)
                    {
                        bufTem[bufTempPos]=readBuf[i];
                        if(readBuf[i]==END_BYTE)
                        {
                            String readMessage = new String(bufTem, 0, bufTempPos);
                            sb.append(readMessage);
                            /*
                            if(sb.toString().equals("temprature reached"))
                            {
                                _mediaPlayer.start();
                            }
                            */
                            textViewWorkingStatus.setText(sb.toString());

                            sb.setLength(0);
                            //sb=new StringBuilder();

                            if(bufTem.length>2)
                            {
                                if(bufTem[0]==(byte)'t' && bufTem[1]==(byte)'p' && bufTem[2]==(byte)'r') //means temprature reached
                                {
                                    _mediaPlayer.start();
                                }
                            }

                            Arrays.fill(bufTem, END_BYTE);
                            //bufTem=new byte[1024];
                            bufTempPos=-1;

                        }
                        bufTempPos++;
                    }



                    //String readMessage = new String(readBuf, 0, msg.arg1);


                   // if (hasEndByte) {
                   //     txtResponse.setText(sb.toString());
                   //     sb=new StringBuilder();
                   // }

                    // construct a string from the valid bytes in the buffer
                    //String readMessage = new String(readBuf, 0, msg.arg1);
                    //txtResponse.setText(readMessage);
                    break;
                case Constants.MESSAGE_READ_END:
                    String text =(String)msg.obj;
                    textViewWorkingStatus.setText(text);

                    // construct a string from the valid bytes in the buffer
                    //String readMessage = new String(readBuf, 0, msg.arg1);
                    //txtResponse.setText(readMessage);
                    break;
                case Constants.MESSAGE_TOAST:
                        Toast.makeText(getApplicationContext(), msg.getData().getString(Constants.TOAST),
                                Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    };

    boolean containsEndByte(byte[] data)
    {
        for (int i=0; i<256; i++)
        {
            if(data[i]==END_BYTE)
            {
                return  true;
            }
        }
        return  false;
    }

    void setConnectionStatusText()
    {
        String text="";

        if(mChatService==null)
            text="Connection not initialized";
        else {
          switch (  mChatService.getState()) {
              case BluetoothChatService.STATE_NONE:
                    text="Not connected";
                  break;
              case BluetoothChatService.STATE_CONNECTING:
                  text="Connecting";
                  break;
              case BluetoothChatService.STATE_CONNECTED:
                  text="Connected";
                  break;
              case  BluetoothChatService.STATE_LISTEN:
                  text="listening for incoming connections";
                  break;
          }
            textViewConnectingStatus.setText(text);
        }
    }

    @Override
    public void onClick(View v) {
        byte[] data = {'c', 0, END_BYTE}; //(byte)255 is -1

        if(v.getId()==R.id.btn10)
        {
            data[1]=10;
        }
        else if(v.getId()==R.id.btn15)
        {
            data[1]=15;
        }
        else if(v.getId()==R.id.btn20)
        {
            data[1]=20;
        }
        else if(v.getId()==R.id.btn25)
        {
            data[1]=25;
        }
        else if(v.getId()==R.id.btn30)
        {
            data[1]=30;
        }
        else if(v.getId()==R.id.btn35)
        {
            data[1]=35;
        }
        else if(v.getId()==R.id.btn40)
        {
            data[1]=40;
        }
        else if(v.getId()==R.id.btn45)
        {
            data[1]=45;
        }
        else if(v.getId()==R.id.btn50)
        {
            data[1]=50;
        }
        else if(v.getId()==R.id.btn55)
        {
            data[1]=55;
        }
        else if(v.getId()==R.id.btn60)
        {
            data[1]=60;
        }
        else if(v.getId()==R.id.btn65)
        {
            data[1]=65;
        }
        if(data[1]>0)
            mChatService.write(data);

    }
}

