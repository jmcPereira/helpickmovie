package com.helpickmovie.hpm;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.ImageView;

/**
 * Created by Pereira on 18-06-2014.
 */
public class ImageView2 extends ImageView
{


    public ImageView2(Context context)
    {
        super(context);
        // TODO Auto-generated constructor stub
        setBackgroundColor(0xFFFFFF);
    }

    public ImageView2(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        // TODO Auto-generated constructor stub
    }


    public ImageView2(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        // TODO Auto-generated constructor stub
    }



    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        // TODO Auto-generated method stub
        Log.d("Hello Android", "Got a touch event: " + event.getAction());
        return super.onTouchEvent(event);

    }
}
