#pragma once

#include<fstream>
#include<string>
#include<iostream>
#include<vector>

using namespace std;

class Time{

public:
	int hh;
	int mm;
	int ss;
	int ms;

	Time(int hh,int mm,int ss,int ms){
		this->hh=hh;
		this->mm=mm;
		this->ss=ss;
		this->ms=ms;
	}

	Time(double time){
		int intTime=(int)time;

		this->hh=intTime/3600;
		intTime%=3600;
		this->mm=intTime/60;
		intTime%=60;
		this->ss=intTime;

		this->ms=(int) (time-(int)time)*1000;
	}

	string toString(){
		string time="";
		time+=hh/10+'0';
		time+=hh%10+'0';
		
		time+=':';

		time+=mm/10+'0';
		time+=mm%10+'0';
		
		time+=':';

		time+=ss/10+'0';
		time+=ss%10+'0';
		
		time+=',';

		time+=ms/100+'0';
		time+=(ms%100)/10+'0';
		time+=ms%10+'0';

		return time;
	}
	
	double toSeconds(){
		return hh*3600+mm*60+ss+(ms*1.0)/1000;
	}
};

vector <Time> times;

bool is_a_time_string(string line){
	for(int i=0;i<line.length();i++){
		if((line[i]>='0' && line[i]<='9')
			|| line[i]==':' || line[i]==',' || line[i]=='\n'
			|| line[i]==' ');
		else
			return false;
	}
	return true;
}

int getTimeInSeconds(string timeString){
	int time=0;
	int len=timeString.length();
	
	for(;len>0 && (timeString[len-1]==' ' || timeString[len-1]=='\n');len--); 
	int pow=0;
	int factor=1;

	for(int i=len-1;i>=0;i--){
		if(timeString[i]==':'){
			pow++;
			factor=1;
			for(int j=0;j<pow;j++){
				factor*=60;
			}
			continue;
		}
		time+=factor*(timeString[i]-'0');
		factor*=10;
	}

	return time;
}

string getSubtitleFileName(string transcriptFileName){
	int len=transcriptFileName.length();

	int lastDotPostion;
	for(lastDotPostion=len-1;lastDotPostion>=0;lastDotPostion--){
		if(transcriptFileName[lastDotPostion]=='.')
			break;
	}

	string subtitleFileName=transcriptFileName.substr(0,lastDotPostion+1);
	subtitleFileName.append("srt");
	return subtitleFileName;
}

bool creatSubtitleFromTranscript(string transcriptFileName){
	
	string line;
	ifstream transcriptFile(transcriptFileName.c_str());

	string subtitleFileName=getSubtitleFileName(transcriptFileName);
	cout<<"Subtitle file: "<<subtitleFileName<<endl;
		
	ofstream subtitleFile(subtitleFileName.c_str());
	
	if (transcriptFile.is_open() && subtitleFile.is_open())
	{
		while ( getline (transcriptFile,line) )
		{
			if(is_a_time_string(line)){
				int timeInSeconds=getTimeInSeconds(line);
				Time aTime((double)timeInSeconds);
				
				times.push_back(aTime);
			}
		}
		
		transcriptFile.clear();
		transcriptFile.seekg(0, ios::beg);

		int count=0;

		while ( getline (transcriptFile,line) )
		{
			if(is_a_time_string(line)){
				
				subtitleFile<<(count+1)<<endl;
				subtitleFile<<times[count].toString()<<" --> ";
				if(count!=times.size()-1){
					int k=times[count].toSeconds();
					times[count]=Time(k+(times[count+1].toSeconds()-k));
				}
				else{
					times[count].ss+=5;
				}
				
				subtitleFile<<times[count].toString()<<endl;

				count++;
			}
			else{
				subtitleFile<<line<<endl<<endl;
			}
		}
		transcriptFile.close();

		subtitleFile.close();

	}

	
	else cout << "Unable to open file"; 

	return true;
}