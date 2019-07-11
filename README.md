# progress-I-rocket-codes
These are the driver codes that will run the arduino mega that controls the uhuru-A engines
The progress-I rocket was designed by makers with reusability in mind.We use the Arduino mega as the core drivers,whereas more intermediate tasks like communication are handled by the UNO.The functionality of the progress-I rocket is very simple.
Takeoff
to execute takeoff procedures the mega;
⦁	Waits for takeoff permission
⦁	uses steppper motors to lock up the landing legs.
⦁	opens extraction valve by 20 degrees to allow atmospheric air replace outgoiing kerosene
⦁	fully open main oxygen valve
⦁	fully open main kerosene valve
⦁	turn on starter motor with relay that routes power from launch pad into the motor to spin turbine
⦁	delay 5 seconds for turbine to get up to full speed
⦁	turn on a sparkplug1 to ignite turbine pump
⦁	slighty open turbine oxygen valve so as not to flood the the small engine with  high pressure gas from pump
⦁	slighty open turbine kerosene valveso as not to flood the the small engine with from highpressure fuel from pump
⦁	delay 6 seconds for turbine to warm up to full speed
⦁	by this time the burner(a high tech matchstick that extends from the launchpad into the combustion chamber) is already lit awaiting fuel - oxygen mixture.
⦁	fully open chamber oxygenvalve to allow oxygen in
⦁	fully open chamber kerosene valve to allow fuel in
⦁	Then ignition
⦁	launch pad clamps delays 3 seconds to allow the rocket gain maximum thrust then releases to allow lift off.

Landing
to execute landing procedures the mega;
⦁	gets data from MPU-6050  accelerometer sensor.
⦁	Uses the data to tilt the craft to fire into the trajectory to slow down.
⦁	gets data from accelerometers.
⦁	gets data to cause cold thrusters to flip the craft the right way up.
⦁	get data from gyro/acelerometer to bring the craft into vertical position for decent using cold thrusters
⦁	gets the starter fluid to react to reignite engine.
⦁	open valves to let fuel and oxygen react to produce thrust and slow the craft for a slow re-entering speed
⦁	gets grid fins to manipulate rocket position during the descent.
⦁	gets the  rocket to riignite again to slow it down
⦁	get a  combination of rocket burns to slow the rocket down a bit
⦁	at a 60 kilometers altitude get the parachute to deploy.
⦁	use gps data to control the accuracy of the descent using vectored-thrust....enabled by using hydraulic or servo gimbals
⦁	Active control can be enabled to control the descent if it falls way off - course
⦁	at 700 metres fire rockets to slow it down to a crawl descent
⦁	at 350 metres unclamp the leg-locks using stepper motors to allow the helium gas unfold the legs.
⦁	At touch down closes all valves,turns off all the devices for shut down.




