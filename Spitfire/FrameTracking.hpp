#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <GLFW/glfw3.h>

class FrameTracker {
public:
	FrameTracker(bool printFrameTime, bool recordFrameTime);
	~FrameTracker();

	void Update();

	bool printToConsole = true;
	bool recordToFile = true;

private:
	void Record();
	void Print();

	double LastFrameTime = 0.0;
	int TotalFrames = 0;
	int TempFrames = 0;	//temperary frames in each second
	time_t startTime = time(0);	//time object so can find date, hour etc
	double loopStartTime = 0.0;	//double just seconds between start time and loop time to find initialising time

};

inline FrameTracker::FrameTracker(bool printFrameTime, bool recordFrameTime) {
	printToConsole = printFrameTime;
	recordToFile = recordFrameTime;
}

inline FrameTracker::~FrameTracker()
{
	if (recordToFile) {
		Record();
	}
}


inline void FrameTracker::Update()
{
	//note when loop started so can calculate startup time
	if (TotalFrames == 0) {
		loopStartTime = glfwGetTime();
	}

	TotalFrames++;

	if (printToConsole) {
		Print(); 
	}
}

inline void FrameTracker::Print()
{
	//Measure frame rate
	double currentTime = glfwGetTime();
	TempFrames++, TotalFrames++;
	if (currentTime - LastFrameTime >= 1.0) {	//
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(TempFrames));
		TempFrames = 0;
		LastFrameTime += 1.0;
	}
}

inline void FrameTracker::Record()
{
	//calculate average frame time and print to console
	double avgFrameTime = ceil((((glfwGetTime() - loopStartTime) * 1000.0) / (double)TotalFrames) * 1000.0) / 1000.0;
	double startupTime = ceil(loopStartTime * 1000) / 1000.0;

	printf("\n\nEnd Stats\nStart-up time %fs\nAverage frame time %fms\nAverage frame rate %f FPS\n", startupTime, avgFrameTime, 1000.0 / avgFrameTime);

	//get local time obj from start time
	tm* localTime = new tm();
	localtime_s(localTime, &startTime);

	//using string stream to write to file
	std::stringstream runTime = std::stringstream();

	//add date and time of start time to string
	runTime << localTime->tm_mday << " " << 1 + localTime->tm_mon << " " << 1900 + localTime->tm_year << " " << localTime->tm_hour << ":" << 0 + localTime->tm_min << " ";

	//add the avg frame time and rate and total run time
	runTime << "Start-up Time: " << startupTime << "s " << "Avg: " << avgFrameTime << "ms " << (int)(1000.0 / avgFrameTime) << "FPS " << "Total: " << glfwGetTime() * 1000.0 << "s\n";

	//read the current contents into the string stream. 
	//means I can write to the start of the file
	std::ifstream currFile;
	currFile.open("Generated/Run-Times.dat");
	runTime << currFile.rdbuf();
	currFile.close();

	//open file to write the run time stats to 
	std::ofstream outfile;
	outfile.open("Generated/Run-Times.dat", std::ios::out);
	outfile << runTime.str();
	outfile.close();
}

