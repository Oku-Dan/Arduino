void setup() {
	Initialize();
	RunToLine();
	FirstGoal();

	for (int i = 0; i < 2;i++){
		ScanBall();
		FirstKeepBall();
		RunExactly(-200);
		Run();
		RunExactly(-150);
		TurnExactly(180);
		TurnExactly(-60);
		Run();
		RunExactly(300);
		RunToLine();
		servo_2.write(45);
		RunExactly(-200);
		TurnExactly(-180);
		Run();
		Run();
	}

	for (int c = 0; c < 6; c++)
	{
		for (int i = 0; i < 6; i++)
		{
			GetBall();
		}

		ReturnToLine();
		
		RunToGoal();
		//Goal();
		//RunFromGoal(); 
		RunExactly(-1000);
	}
}
void loop() {
	/*
	if(ScanBall())
		KeepBall();
	delay(5000);*/
	//TurbineStart(); 
}
