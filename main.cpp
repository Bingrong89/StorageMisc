#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace cv;
using namespace std;
using namespace boost::filesystem;

Point poiA;
Point poiB;
bool firstmove = false;
bool classseq = false;

string class1 = "Line";
string class2 = "Pie";
string class3 = "Vertical Histobar";
string class4 = "Horizontal Histobar";
vector<string> classlist{class1,class2,class3,class4};

//Make sure naming convention is correct.. i will idiotproof this though
string savingfolder = "/home/bing/Documents/AFRDE-Data/Trial1";
string class1folder = savingfolder + "/" + class1;
string class2folder = savingfolder + "/" + class2;
string class3folder = savingfolder + "/" + class3;
string class4folder = savingfolder + "/" + class4;
vector<string> folderlist ={class1folder,class2folder,class3folder,class4folder};


//prompt user to input number to get class index for vector usage
int Getobjclass(){
    int clsnumber;
    cout<<"Input class category"<<endl; //swap with fixed choice
    for(int i=0;i<classlist.size();++i){ //display classe names in console
        cout<< i+1<<" : " << classlist[i]<<endl;
    }
    cin>>clsnumber;
    if(clsnumber >classlist.size()){
        throw invalid_argument("INVALID CLASS NUMBER");
    }
    return clsnumber;
}

// class to hold vertexes and class index for vector usage
// I do not want to include those info within the class definition so i will always see them
class classinfoholder{
public:
    Point vertex1;
    Point vertex2;
    int classnumba;
    classinfoholder( Point A, Point B, int inputnumba){
    vertex1 = A;
    vertex2 =B;
    classnumba = inputnumba;
    }

    void printinfo(){
    cout<<"1st vertex = "<<vertex1 <<"2nd vertex = "<<vertex2
    <<"Class name is:" <<classlist[classnumba-1] <<endl;
    }

};

// output annotation files
void writexmlfile(string* file_name,Mat* srcimage,vector<classinfoholder>* allinfo,
                  vector<string>* classlist, int i){
    std::ofstream myfile;
    myfile.open(*file_name);
    myfile<< "<annotation>";
    myfile<< "<folder>"<<"Data"<<"</folder>";  //FOLDER THAT CONTAINS ANNOTATION FOLDER, NOT THAT IT MATTERS, PATHING FROM LOADING FILE
    myfile <<"<size>";
    myfile <<"<width>"<<(*srcimage).cols<<"</width>"; //IMAGE WIDTH
    myfile <<"<height>"<<(*srcimage).rows<<"</height>"; //IMAGE HEIGHT
    myfile<<"<depth>"<< "3" <<"</depth>";  //IMAGE COLOR CHANNELS
    myfile<<"</size>";
    myfile<<"<object>";
    myfile<<"<name>"<<(*classlist)[((*allinfo)[i]).classnumba-1]<<"</name>";  //object CLASS NAME
    myfile<<"<bndbox>";
    myfile<<"<xmin>"<<(*allinfo)[i].vertex1.x<<"</xmin>"; //X1
    myfile<<"<ymin>"<<(*allinfo)[i].vertex1.y<<"</ymin>"; //Y1
    myfile<<"<xmax>"<<(*allinfo)[i].vertex2.x<<"</xmax>"; //X2;
    myfile<<"<ymax>"<<(*allinfo)[i].vertex2.y<<"</ymax>";  //Y2
    myfile<<"</bndbox>";
    myfile<<"</object>";
    myfile<<"</annotation>";
    myfile.close();
                  }


//trigger events from mouse activities
void Mouseevents(int event, int x, int y, int flags, void* param){
        Mat& image = *(cv::Mat*) param;
        switch(event) {
        case EVENT_LBUTTONDOWN:{
        ::poiA= Point(x,y);
        firstmove = true;
        }break;

        case EVENT_LBUTTONUP: {
            ::poiB = Point(x,y);
            rectangle(image, poiA,poiB, Scalar(100,200,300),2);
            classseq = true;
        }break;
        case EVENT_MOUSEMOVE: {
            if(firstmove== false){
                break;
            }
            else{
            //rectangle(image, poiA,Point(x,y), Scalar(100,200,300), 5);
            }break;

        }//break;
    }
}

// generate image seq number, for consistent file naming purposes across all classes
int imagenumberseq(string* universalimagefolder){
        path xit(*universalimagefolder);
    directory_iterator xit2(xit);
    directory_iterator end;
    int counter = 1; //==+1 after counting finish, because new file number always =currenttotal++
    for (xit2;xit2!=end;++xit2){counter++;}
    return counter;
}

//draw respective output images by filling other object class areas white
void outputimage(Mat* leimage,vector<classinfoholder>*vectorofall, int i){
    for(int z=0;z<(*vectorofall).size();++z){
        if(z==i){
            continue;
        }
        else{
            rectangle(*leimage,(*vectorofall)[z].vertex1,(*vectorofall)[z].vertex2,
                      Scalar(255,255,255),CV_FILLED);
        }
    }
    }


int main(){
    string inputimage = "/home/bing/Documents/AFRDE-Data/Trial1/Images/.jpg";
	Mat srcImage = imread(inputimage, 1);
	Mat drawimg =  imread(inputimage, 1);
	Mat tempImage;
	vector<classinfoholder>allinfo;

	string WINDOW_NAME = "YOO";
    namedWindow(WINDOW_NAME);
    imshow(WINDOW_NAME, srcImage);
	setMouseCallback(WINDOW_NAME, Mouseevents, (void*) &srcImage);

while (true){
        srcImage.copyTo(tempImage);
		imshow(WINDOW_NAME, tempImage);

		if (classseq == true){     // trigger loop upon finishing one rectangle
            classseq = false;
            int a1 = Getobjclass();
            allinfo.push_back(classinfoholder(poiA,poiB,a1));
		}

		if (waitKey(10) == 27)  // stop drawing rectangle if the key is 'ESC'
        {
            for(int i = 0;i<allinfo.size();++i){
                allinfo[i].printinfo();
            }
            break;
        }
	}

	//number sequencing is based on a universal folder of all created images)
	string imagesfolder = "/home/bing/Documents/AFRDE-Data/Trial1/All/Images";

	for (int i=0;i<allinfo.size();++i){  //trigger writing of xml files and saving of images
    int newimgnumber = imagenumberseq(&imagesfolder);

    string filecounter = boost::lexical_cast<string>(newimgnumber);
    string xmlfilename = folderlist[allinfo[i].classnumba-1] +"/Annotation/" + filecounter + ".xml";

    writexmlfile(&xmlfilename,&srcImage,&allinfo,&classlist,i);
    outputimage(&drawimg,&allinfo,i);

    string writeimgname = folderlist[allinfo[i].classnumba-1] +"/Images/" + filecounter + ".jpg";
    string writeallimg = imagesfolder + "/" + filecounter + ".jpg";
    imwrite(writeimgname,drawimg);
    imwrite(writeallimg,drawimg);
    drawimg = imread(inputimage, 1);
	} //end of FOR loop

//      remove(inputimage);  //YUPPP

}


