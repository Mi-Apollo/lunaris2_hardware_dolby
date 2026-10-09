package org.lineageos.dspvolume.xiaomi;

import android.app.Service;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.os.IBinder;

import androidx.annotation.Nullable;

public class VolumeListenerService extends Service {
    private VolumeListenerReceiver volumeReceiver;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("android.media.VOLUME_CHANGED_ACTION");
        volumeReceiver = new VolumeListenerReceiver();
        registerReceiver(volumeReceiver, intentFilter);

        AudioManager audioManager = getSystemService(AudioManager.class);
        int current = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
        audioManager.setParameters("volume_change=" + current + ";flags=8");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (volumeReceiver != null) {
            unregisterReceiver(volumeReceiver);
            volumeReceiver = null;
        }
        super.onDestroy();
    }
}
