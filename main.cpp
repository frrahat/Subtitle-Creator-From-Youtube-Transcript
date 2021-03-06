#include "OpenFileDialog.h"
#include "SubtitleCreator.h"
#include<iostream>

using namespace std;

int main(){
	OpenFileDialog* openFileDialog1 = new OpenFileDialog();
	
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->Flags |= OFN_SHOWHELP;
	//openFileDialog1->InitialDir = _T("C:\\");
	openFileDialog1->Title = _T("Open Text File");

	if (openFileDialog1->ShowDialog())
	{
		MessageBox(0, openFileDialog1->FileName, _T("Confirm Selection"),
				   MB_OK | MB_ICONINFORMATION);

		cout<<openFileDialog1->FileName<<endl;
		creatSubtitleFromTranscript(openFileDialog1->FileName);

		MessageBox(0, "Check the same directory for the subtitle file.",_T("Task Finished"),MB_OK);
	}
	
	return 0;
}