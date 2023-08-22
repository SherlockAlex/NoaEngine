#include "NoaEngine.h"

using namespace noa;

class RetroRacer:public NoaGameEngine
{
private:
	float carPos = 0.0f;
	float fDistance = 0.0f;
	float fSpeed = 0.0f;
	float fTurnSpeed = 1.0f;

	float fTrackCurvature = 0.0f;
	float fCurvature = 0.0f;
	float fPlayerCurvature = 0.0f;
	float fTrackDistance = 0.0f;

	vector<pair<float, float>> vecTrack; //<curvature,distance>

	Sprite carSprite;
	Audio BGM = Audio("./Assets/RetroRacer/BGM.mp3", AudioType::Music);

public:
	RetroRacer(int width,int height,GameWindowMode windowMode,string name) : NoaGameEngine(width,height, windowMode,name)
	{
		carSprite = Sprite(LoadSprFile("./Assets/RetroRacer/car.spr"), 10);
	}
	~RetroRacer() {

	}

public:
	void Start() override {
		vecTrack.push_back(make_pair(0.0f, 10.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 400.0f));
		vecTrack.push_back(make_pair(-1.0f,100.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(-1.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(0.2f, 500.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));

		for (auto t:vecTrack) {
			fTrackDistance += t.second;
		}

		BGM.Play(true);

	}

	void Update() override {

		if (inputSystem.GetKeyHold(KeyW))
		{
			fSpeed += 2.0 * deltaTime;
		}
		else {
			fSpeed -= 1.0 * deltaTime;
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			fPlayerCurvature -= fTurnSpeed * deltaTime;
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			fPlayerCurvature += fTurnSpeed * deltaTime;
		}

		if (fabs(fPlayerCurvature - fTrackCurvature)>=0.8f) 
		{
			fSpeed -= 0.5f * deltaTime;
		}

		if (fSpeed<0.0f) 
		{
			fSpeed = 0.0f;
		}
		if (fSpeed>2.0f)
		{
			fSpeed = 2.0f;
		}

		fDistance += (70.0f * fSpeed) * deltaTime;

		float fOffset = 0;
		int nTrackSection = 0;

		if (fDistance>=fTrackDistance)
		{
			fDistance -= fTrackDistance;
		}

		while (nTrackSection<vecTrack.size()&&fOffset<=fDistance)
		{
			fOffset += vecTrack[nTrackSection].second;
			nTrackSection++;
		}

		float fTargetCurvature = vecTrack[nTrackSection - 1].first;

		float fTrackCurveDiff = (fTargetCurvature - fCurvature) * deltaTime * fSpeed;
		fCurvature += fTrackCurveDiff;

		fTrackCurvature += (fCurvature)*deltaTime * fSpeed;

		for (int y = 0;y<pixelHeight/2;y++)
		{
			for (int x = 0;x<pixelWidth;x++) 
			{

				Uint32 color = BLACK;

				//近大远小
				float fPerspective = (float)y / (pixelHeight / 2.0f);

				float fMiddlePoint = 0.5f + fCurvature * powf((1.0f-fPerspective),3);
				float fRoadWidth =0.1f + fPerspective * 0.8f;
				float fClipWidth = fRoadWidth * 0.15f;

				fRoadWidth *= 0.5f;

				int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * pixelWidth;
				int nLeftClip = (fMiddlePoint - fRoadWidth) * pixelWidth;
				int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * pixelWidth;
				int nRightClip = (fMiddlePoint + fRoadWidth) * pixelWidth;

				int nRow = pixelHeight / 2 + y;

				Uint32 nGrassColor = sinf(20.0 * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f?GREEN:RGB(GetRValue(GREEN)/2,GetGValue(GREEN)/2, GetBValue(GREEN) /2);
				Uint32 nClipColor = sinf(80.0 * powf(1.0f - fPerspective, 2) + fDistance ) > 0.0f ? RED:WHITE;

				int nRoadColor = DARKGRAY;

				if (x>=0&&x<nLeftGrass)
				{
					color = nGrassColor;
					//renderer.DrawPixel(x, nRow, nGrassColor);
				}
				else if (x>=nLeftGrass&&x<nLeftClip)
				{
					color = nClipColor;
					//renderer.DrawPixel(x, nRow, nClipColor);
				}
				else if (x >= nLeftClip&& x < nRightClip)
				{
					color = nRoadColor;
					//renderer.DrawPixel(x, nRow, DARKGRAY);
				}
				else if (x >= nRightClip && x < nRightGrass)
				{
					color = nClipColor;
					//renderer.DrawPixel(x, nRow, nClipColor);
				}
				else if (x >= nRightGrass && x < pixelWidth)
				{
					color = nGrassColor;
					//renderer.DrawPixel(x, nRow, nGrassColor);
				}
				renderer.DrawPixel(x, nRow, color);
			}
		}

		//Draw Car
		carPos = fPlayerCurvature - fTrackCurvature;
		if (carPos<-0.85f||carPos>0.85f)
		{
			fSpeed -= 1.0 * deltaTime;
		}
		int nCarPos = pixelWidth / 2 + ((int)(pixelWidth * carPos) / 2.0f) - 7;
		int nCarY = pixelHeight*0.75;

		carSprite.DrawSprite(nCarPos, nCarY, true);

		//绘制car sprite
	}

};

int main(int argc,char * argv[])
{
	RetroRacer game(1920/2,1080/2, NoaGameEngine::WindowMode,"RetroRacer");
	game.Run();
	return 0;
}