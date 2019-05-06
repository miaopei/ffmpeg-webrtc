package com.webrtc.android.avcall.signal;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.net.URISyntaxException;

import io.socket.client.IO;
import io.socket.client.Socket;
import io.socket.emitter.Emitter;

public class SignalClient {

    private static final String TAG = "SignalClient";

    private static SignalClient mInstance;
    private OnSignalEventListener mOnSignalEventListener;

    private Socket mSocket;
    private String mRoomName;

    public interface OnSignalEventListener {
        void onConnected();
        void onConnecting();
        void onDisconnected();
        void onUserJoined(String roomName, String userID);
        void onUserLeaved(String roomName, String userID);
        void onRemoteUserJoined(String roomName);
        void onRemoteUserLeaved(String roomName, String userID);
        void onRoomFull(String roomName, String userID);
        void onMessage(JSONObject message);
    }

    public static SignalClient getInstance() {
        synchronized (SignalClient.class) {
            if (mInstance == null) {
                mInstance = new SignalClient();
            }
        }
        return mInstance;
    }

    public void setSignalEventListener(final OnSignalEventListener listener) {
        mOnSignalEventListener = listener;
    }

    public void joinRoom(String url, String roomName) {
        Log.i(TAG, "joinRoom: " + url + ", " + roomName);
        try {
            mSocket = IO.socket(url);
            mSocket.connect();
        } catch (URISyntaxException e) {
            e.printStackTrace();
            return;
        }
        //mUserId = userId;
        mRoomName = roomName;
        listenSignalEvents();

        mSocket.emit("join", mRoomName);
    }

    public void leaveRoom() {

        Log.i(TAG, "leaveRoom: " + mRoomName);
        if (mSocket == null) {
            return;
        }

        mSocket.emit("leave", mRoomName);
        mSocket.close();
        mSocket = null;
    }

    public void sendMessage(JSONObject message) {
        Log.i(TAG, "broadcast: " + message);
        if (mSocket == null) {
            return;
        }
        mSocket.emit("message", mRoomName, message);
    }

    //侦听从服务器收到的消息
    private void listenSignalEvents() {

        if (mSocket == null) {
            return;
        }

        mSocket.on(Socket.EVENT_CONNECT_ERROR, new Emitter.Listener() {
            @Override
            public void call(Object... args) {

                Log.e(TAG, "onConnectError: " + args);
            }
        });

        mSocket.on(Socket.EVENT_ERROR, new Emitter.Listener() {
            @Override
            public void call(Object... args) {

                Log.e(TAG, "onError: " + args);
            }
        });

        mSocket.on(Socket.EVENT_CONNECT, new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                String sessionId = mSocket.id();
                Log.i(TAG, "onConnected");
                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onConnected();
                }
            }
        });

        mSocket.on(Socket.EVENT_CONNECTING, new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                Log.i(TAG, "onConnecting");
                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onConnecting();
                }
            }
        });

        mSocket.on(Socket.EVENT_DISCONNECT, new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                Log.i(TAG, "onDisconnected");
                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onDisconnected();
                }
            }
        });

        mSocket.on("joined", new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                String roomName = (String) args[0];
                String userId = (String) args[1];
                if (/*!mUserId.equals(userId) &&*/ mOnSignalEventListener != null) {
                    //mOnSignalEventListener.onRemoteUserJoined(userId);
                    mOnSignalEventListener.onUserJoined(roomName, userId);
                }
                //Log.i(TAG, "onRemoteUserJoined: " + userId);
                Log.i(TAG, "onUserJoined, room:" + roomName + "uid:" + userId);
            }
        });

        mSocket.on("leaved", new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                String roomName = (String) args[0];
                String userId = (String) args[1];
                if (/*!mUserId.equals(userId) &&*/ mOnSignalEventListener != null) {
                    //mOnSignalEventListener.onRemoteUserLeft(userId);
                    mOnSignalEventListener.onUserLeaved(roomName, userId);
                }
                Log.i(TAG, "onUserLeaved, room:" + roomName + "uid:" + userId);
            }
        });

        mSocket.on("otherjoin", new Emitter.Listener() {

            @Override
            public void call(Object... args) {
                String roomName = (String) args[0];
                String userId = (String) args[1];
                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onRemoteUserJoined(roomName);
                }
                Log.i(TAG, "onRemoteUserJoined, room:" + roomName + "uid:" + userId);
            }
        });

        mSocket.on("bye", new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                String roomName = (String) args[0];
                String userId = (String) args[1];
                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onRemoteUserLeaved(roomName, userId);
                }
                Log.i(TAG, "onRemoteUserLeaved, room:" + roomName + "uid:" + userId);

            }
        });

        mSocket.on("full", new Emitter.Listener() {
            @Override
            public void call(Object... args) {

                //释放资源
                mSocket.disconnect();
                mSocket.close();
                mSocket = null;

                String roomName = (String) args[0];
                String userId = (String) args[1];

                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onRoomFull(roomName, userId);
                }

                Log.i(TAG, "onRoomFull, room:" + roomName + "uid:" + userId);

            }
        });

        mSocket.on("message", new Emitter.Listener() {
            @Override
            public void call(Object... args) {
                String roomName = (String)args[0];
                JSONObject msg = (JSONObject) args[1];

                if (mOnSignalEventListener != null) {
                    mOnSignalEventListener.onMessage(msg);
                }

                Log.i(TAG, "onMessage, room:" + roomName + "data:" + msg);

            }
        });
    }
}
