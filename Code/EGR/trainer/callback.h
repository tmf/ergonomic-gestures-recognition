#pragma once
class trainer;
void *instance;

void __cdecl EGRTrackerFinished(void){
	((trainer *)instance)->callback();
}