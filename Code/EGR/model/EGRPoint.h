#pragma once

struct EGRPoint {
	EGRPoint(const EGRPoint &p) : x(p.x), y(p.y), vx(p.vx), vy(p.vy), frame(p.frame), lastRealFrame(p.lastRealFrame), estimated(p.estimated), angle(p.angle), angle2(p.angle2), centerx(p.centerx), centery(p.centery){}
	EGRPoint(int _x, int _y, int _vx, int _vy, int _frame, int _lastRealFrame, bool _estimated, float _angle, float _angle2, int _centerx, int _centery):
x(_x), y(_y), vx(_vx), vy(_vy), frame(_frame), lastRealFrame(_lastRealFrame), estimated(_estimated), angle(_angle), angle2(_angle2), centerx(_centerx), centery(_centery){}
EGRPoint(): x(0), y(0), vx(0), vy(0), frame(0), lastRealFrame(0), estimated(true), angle(0.0), angle2(0.0), centerx(0), centery(0){}
	int x;
	int y;
	int vx;
	int vy;
	int frame;
	int lastRealFrame;
	bool estimated;
	float angle;
	float angle2;
	int centerx;
	int centery;
};