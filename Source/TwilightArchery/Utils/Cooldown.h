#pragma once

DECLARE_MULTICAST_DELEGATE(FCooldownCallbackSignature);

/**
 *	Cooldown helper class.
 **/
class FCooldown
{
public:
	/**
	 *	Constructor
	 *	@param Duration Number of seconds that must pass until the cooldown is done
	 **/
	FCooldown(const float& Duration)
		: CurrentDeltaTime(0.0f)
		, CurrentTime(0.0f)
		, CooldownDuration(Duration)
	{
	}

	/**
	 *	Returns the current time, in seconds.
	 *	@return Current time, in seconds
	 */
	float GetCurrentTime() const
	{
		return CurrentTime;
	}

	/**
	 *	Returns the current completion percentage, between 0 and 1.
	 *	@return Current CompletionPercentage, between 0 and 1.
	 */
	float GetCompletionPercent() const
	{
		if (CurrentTime > 0)
			return 1 - CurrentTime / CooldownDuration;
		return 0;
	}

	/**
	 *	Returns the current delta time.
	 *	@return CurrentDeltaTime (number of seconds that passed between the last two tick)
	 */
	float GetCurrentDeltaTime() const
	{
		return CurrentDeltaTime;
	}

	/**
	 *	Returns the cooldown duration.
	 *	@return CooldownDuration (number of seconds that must pass until the cooldown is done)
	 */
	float GetCooldownDuration() const
	{
		return CooldownDuration;
	}

	/**
	 *	Sets the cooldown duration.
	 *  @param TargetDuration Number of seconds that must pass until the cooldown is done
	 **/
	void SetDuration(const float& TargetDuration)
	{
		CooldownDuration = TargetDuration;
	}

	/**
	 *	Checks if the cooldown has started or not.
	 *	@return true if the cooldown has started, and false if it hasn't
	 */
	bool HasStarted() const
	{
		return CurrentTime > 0;
	}

	/**
	 *	Checks if the cooldown has finished or not.
	 *	@return true if the cooldown has finished, and false if it hasn't
	 */
	bool HasFinished() const
	{
		return CurrentTime <= 0;
	}

	/**
	 *	Starts the cooldown (sets CurrentTime to CooldownDuration and ticks down when the Tick method is called).
	 **/
	void Start()
	{
		CurrentTime = CooldownDuration;
	}

	/**
	 *	Stops the cooldown (sets CurrentTime 0 and broadcasts CooldownCallback).
	 **/
	void Stop(const bool& DoCallback = true)
	{
		if (HasStarted() && DoCallback)
			CooldownCallback.Broadcast();
		CurrentTime = 0;
	}
	
	/**
	 *	Updates the cooldown.
	 *	@param DeltaTime Number of seconds that have passed since the last tick
	 **/
	void Tick( const float& DeltaTime )
	{
		CurrentDeltaTime = DeltaTime;
		if (CurrentTime > 0) {
			CurrentTime -= DeltaTime;
			if (CurrentTime <= 0) {
				CooldownCallback.Broadcast();
				CurrentTime = 0;
			}
		}
	}

	/** Delegate executed when the cooldown reaches 0. */
	FCooldownCallbackSignature CooldownCallback;

protected:
	/** Current delta time (number of seconds that passed between the last two tick). */
	float CurrentDeltaTime;
	/** Current time, in seconds. */
	float CurrentTime;
	/** Cooldown duration, in seconds. */
	float CooldownDuration;
};
