
void syncvid (VideoCapture& cap1, VideoCapture& cap2) 
{ //right now manual. have to do something to control cameras
    Mat frame1, frame2;
    char c='g';
    cap1.read(frame1);
    cap2.read(frame2);
    while(c!='q') 
    {
        if(c=='1')
            cap1.read(frame1);
        if(c=='2') 
            cap2.read(frame2);
        imshow("frame1",frame1);
        imshow("frame2",frame2);
        c=waitKey(0);
    }
}
