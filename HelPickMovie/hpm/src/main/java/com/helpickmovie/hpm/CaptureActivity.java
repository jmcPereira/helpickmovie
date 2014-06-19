package com.helpickmovie.hpm;

import android.app.Activity;
import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.NetworkOnMainThreadException;
import android.os.StrictMode;
import android.provider.MediaStore;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;


public class CaptureActivity extends Activity {
    private static final int REQUEST_IMAGE_CAPTURE = 1;
    private ImageView mImageView;
    private String mCurrentPhotoPath;
    private static final int REQUEST_TAKE_PHOTO = 1;
    private MediaScannerConnection msc;
    private Bitmap bitmap;

    private int drawing;
    private float first_x;
    private float first_y;
    private float last_x;
    private float last_y;
    int desenho;

    private float leftx ;
    private float topy ;
    private float rightx ;
    private float bottomy ;

    Canvas canvas;
    Bitmap mbitmap;
    Bitmap save;
    Bitmap cropped;

    String encodedImage;

    final int PIC_CROP = 2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_capture);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();

        StrictMode.setThreadPolicy(policy);

        mImageView = (ImageView) findViewById(R.id.frame);
        desenho=0;
        final Button undo = (Button) findViewById(R.id.undo_rectangle);
        final Button send =(Button) findViewById(R.id.envia);
        Context context = getApplicationContext();
        CharSequence text = "Select Poster first please!";
        int duration = Toast.LENGTH_SHORT;

        final Toast toast = Toast.makeText(context, text, duration);

        undo.setOnClickListener(new View.OnClickListener() {

            public void onClick(View v) {
               restoreBitmap();
            }
        });
        send.setOnClickListener(new View.OnClickListener() {

                                    public void onClick(View v) {
                                        if(desenho==1){
                                            Log.d("VALUES","LEFT X: "+leftx+"\nTOP Y: "+topy+"\nRIGHT X: "+rightx+"\nBOTTOM Y: "+bottomy);
                                            cropped = Bitmap.createBitmap(mbitmap,   (int)leftx+3,(int)bottomy+3,(int)rightx-(int)leftx-6 ,(int)topy-(int)bottomy-6);

                                            mImageView.setImageBitmap(cropped);
                                            sendP();


                                        }
                                        else {
                                            toast.show();
                                        }
                                    }
                                });
        dispatchTakePictureIntent();



    }


    private void dispatchTakePictureIntent() {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        // Ensure that there's a camera activity to handle the intent
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            // Create the File where the photo should go
            File photoFile = null;
            try {
                photoFile = createImageFile();
            } catch (IOException ex) {
                Log.d("ERROR","Error creating photoFile!");

            }
            // Continue only if the File was successfully created
            if (photoFile != null) {

                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
                        Uri.fromFile(photoFile));
                startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
            }
        }
    }

    private File createImageFile() throws IOException {
        // Create an image file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(
                imageFileName,  /* prefix */
                ".jpg",         /* suffix */
                storageDir      /* directory */
        );

        // Save a file: path for use with ACTION_VIEW intents
        mCurrentPhotoPath = image.getAbsolutePath();
        return image;
    }

    private void setPic() {

        DisplayMetrics size = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(size );
        int w = size .widthPixels;
        int h = size .heightPixels;

        // Get the dimensions of the View
        int targetW = w;
        int targetH = h;
        // Get the dimensions of the bitmap
        BitmapFactory.Options bmOptions = new BitmapFactory.Options();
        bmOptions.inJustDecodeBounds = true;
        BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
        int photoW = bmOptions.outWidth;
        int photoH = bmOptions.outHeight;
        // Determine how much to scale down the image
        int scaleFactor = Math.min(photoW/targetW, photoH/targetH);
        // Decode the image file into a Bitmap sized to fill the View
        bmOptions.inJustDecodeBounds = false;
        bmOptions.inSampleSize = scaleFactor;
        bmOptions.inPurgeable = true;
        bitmap = BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
        //Bitmap rbitmap=RotateBitmap(bitmap,90);
        mImageView.setImageBitmap(bitmap);
        last_x=0;
        last_y=0;
        first_y=0;
        first_x=0;
        mImageView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(desenho==0) {
                    float x = event.getX();
                    float y = event.getY();
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            first_y = y;
                            first_x = x;
                            Log.d("EVENT", "TOUCH DOWN");
                            break;
                        case MotionEvent.ACTION_UP:
                            last_x = x;
                            last_y = y;
                            Log.d("EVENT", "TOUCH UP");
                            mark_crops();
                            desenho=1;
                            break;
                    }
                }
                return true;
            }
        });

    }

    private void galleryAddPic() {
        Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        File f = new File(mCurrentPhotoPath);
        Uri contentUri = Uri.fromFile(f);
        mediaScanIntent.setData(contentUri);
        this.sendBroadcast(mediaScanIntent);
    }



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        handleBigCameraPhoto();
    }


    public static Bitmap RotateBitmap(Bitmap source, float angle)
    {
        Matrix matrix = new Matrix();
        matrix.postRotate(angle);
        return Bitmap.createBitmap(source, 0, 0, source.getWidth(), source.getHeight(), matrix, true);
    }

    private void handleBigCameraPhoto() {

        if (mCurrentPhotoPath != null) {
            setPic();
            galleryAddPic();
            mbitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
            save=bitmap.copy(Bitmap.Config.ARGB_8888, true);
            canvas = new Canvas(mbitmap);

        }

    }

    public void restoreBitmap(){
        mbitmap=save.copy(Bitmap.Config.ARGB_8888, true);
        desenho=0;
        mImageView.setImageBitmap(mbitmap);
        canvas = new Canvas(mbitmap);
    }

    public void mark_crops(){
        Paint paint = new Paint();
        paint.setColor(Color.GREEN);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(2);
        leftx = 20;
        topy = 20;
        rightx = 50;
        bottomy = 100;


        int imvh=mImageView.getHeight();
        int imvw=mImageView.getWidth();
        int bh=mbitmap.getHeight();
        int bw=mbitmap.getWidth();

        int fx2= (int) ((bw*first_x)/imvw);
        int fy2= (int) ((bh*first_y)/imvh);

        int lx2= (int) ((bw*last_x)/imvw);
        int ly2= (int) ((bh*last_y)/imvh);

        Log.d("VALUES:\n","   ImageView Height: "+imvh+"\n   ImageView Width: "+imvw+"\n   Bitmap Height: "+bh+"\n   Bitmap Width: "+bw+"\n   Fx: "+fx2+"\n   Fy: "+fy2+"\n   Lx: "+lx2+"\n   Ly: "+ly2+
        "\n   First X: "+first_x+"\n   First Y: "+first_y+"\n   Last X: "+last_x+"\n   Last Y: "+last_y);

        if(first_x>last_x && first_y>last_y) {
            rightx = fx2;
            leftx = lx2;
            topy=fy2;
            bottomy=ly2;
        }
        if(last_x>first_x && first_y<last_y) {
            rightx = lx2;
            leftx = fx2;
            topy=ly2;
            bottomy=fy2;
        }


        canvas.drawRect(leftx, topy, rightx, bottomy, paint);
        mImageView.setImageBitmap(mbitmap);

    }



    public static String encodeTobase64(Bitmap image)
    {
        Bitmap immagex=image;
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        immagex.compress(Bitmap.CompressFormat.JPEG, 100, baos);
        byte[] b = baos.toByteArray();
        String imageEncoded = Base64.encodeToString(b,Base64.DEFAULT);

        Log.e("LOOK", imageEncoded);
        return imageEncoded;
    }

    private void sendP(){
        String encodedbitmap=encodeTobase64(cropped);
        HttpClient client = new DefaultHttpClient();
        HttpPost post = new HttpPost( "http://172.19.105.67/receptor.php");
        try {
            // Add your data
            List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(2);
            nameValuePairs.add(new BasicNameValuePair("imagem", encodedbitmap));
            post.setEntity(new UrlEncodedFormEntity(nameValuePairs));

            // Execute HTTP Post Request
            HttpResponse response = client.execute(post);
            String aaa=response.getStatusLine().getReasonPhrase();
            int status = response.getStatusLine().getStatusCode();
            if(status==200){
                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(aaa));
                startActivity(browserIntent);
            }
            Log.d("Status :", ""+status+"  reason: "+aaa);
        } catch (ClientProtocolException e) {
            Log.e("Exception :", e.getMessage());
        } catch (IOException e) {
            Log.e("Exception :", e.getMessage());
        }

    }

}
