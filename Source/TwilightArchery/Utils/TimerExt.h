#pragma once

/**
 *  Starts the given timer by adding a small value to its current time.
 **/
inline void StartTimer(FTimer& Timer)
{
	Timer.Tick(0.0001);
}

/**
 *  Checks if the given timer has started.
 *	@return true if the timer has started, and false if it hasn't
 **/
inline bool HasTimerStarted(const FTimer& Timer)
{
	return Timer.GetCurrentTime() > 0;
}

/**
 *	Timer Extensions helper class.
 **/
class FTimerExt : public FTimer
{
public:
	/**
	 *  Starts the timer by adding a small value to its current time.
	 **/
	void Start()
	{
		CurrentTime += 0.0001;
	}
	
	/**
	 *  Restarts the timer by setting its current time to a small value.
	 **/
	void Restart()
	{
		CurrentTime = 0.0001;
	}
	
	/**
	 *  Resets the timer's current time to 0.
	 **/
	void Reset()
	{
		CurrentTime = 0;
	}
	
	/**
	 *  Sets the timer's current time to the given value.
	 *  @param NewTime Number of seconds to set CurrentTime to.
	 **/
	void SetCurrentTime(const float& NewTime)
	{
		CurrentTime = NewTime;
	}
	
	/**
	 *  Checks if the timer has started.
	 *	@return true if the timer has started, and false if it hasn't
	 **/
	bool HasStarted() const
	{
		return CurrentTime > 0;
	}
	
	/**
	 *	Updates the timer. Will not tick if the timer hasn't started.
	 *	@param DeltaTime Number of seconds that have passed since the last tick
	 **/
	void Tick(float DeltaTime)
	{
		CurrentDeltaTime = DeltaTime;
		if (HasStarted())
		{
			CurrentTime += DeltaTime;
			if (CurrentTime < 0)
				CurrentTime = 0;
		}
	}
};