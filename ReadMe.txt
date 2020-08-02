1.
The database could not be uploaded on intinno due to size limit as its size is 
1.4 GB. I am uploading the database online in google drive. The link will be
mailed. Untar in the folder where src folder is present.

2.
To run the program type the following on console

$ g++ `pkg-config --cflags opencv` -I ./ main.cpp Methods.cpp Classifier.cpp `pkg-config --libs opencv`
$ ./a.out

This will show the result something as follows:
total no of images read: 9
[0] genuine
[1] genuine
[2] genuine
[3] genuine
[4] genuine
[5] genuine
[6] genuine
[7] genuine
[8] genuine
genuine : 9
forged : 0
......

3.
By default the TEST_PATH in main.cpp is given to simulated sample folder. 
To verify result it for other samples such as genuine or disguise make following
changes in main.cpp

Change TEST_PATH to ../data/Genuine for genuine sample and
				    ../data/Disguise for disguise sample

4. 
To view the sub-image cells formed for every signature image set _DEBUG 1 in
main.cpp 

5.
The results obtained for every image in each sample is present in Results.txt

6. 
The signature database used is present in data folder
