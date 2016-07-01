cd final\ structure/
./detection data/camera1/calib1.xml data/camera2/calib2.xml ../../dslr2/camera1/2.mp4 ../../dslr2/camera2/2.mp4 data/output.txt < params.txt;
cd ../interpolation;
make;
./spline_fit_real 2109 2145 2133;
./bounce_area;
cd plots;
python plotData.py;
python plotPos.py;
echo Determining bounce area;
echo .......;
echo .......;
echo Sit back and relax;
python plotArea.py;
cd ../..;
