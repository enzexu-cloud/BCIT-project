Part 1: Three-Floor Elevator

Program the PLC elevator to respond to calls made from floors 1-3.

• Once run, the elevator should move to the Floor 1 and wait.

• The elevator can be called to a corresponding floor with FPB1, FPB2, and FPB3.

• FIL1, FIL2, and FIL3 should notify the user that the call is accepted.

• The elevator should stop at the floor it was called to and wait for the next call.

• The elevator never moves below Floor 1 or above Floor 3.

Part 2: Inside Panel

Program the PLC elevator to respond to requests to move to one of three floors.

• Passengers can request to go to a corresponding floor with PB1, PB2, and PB3.

• IL1, IL2, and IL3 should notify the passenger the request is accepted.

Part 3: Elevator Door

Program the PLC elevator to open and close the elevator door to allow passengers on.

• The elevator door opens for 5 seconds when servicing a floor.

• After 2 seconds, the door can be closed by pressing PBC on the inside panel.

• Pressing PBO should hold the door open for another 3 seconds.

• If the elevator is waiting for a floor call, the door remains closed.

• If the elevator is called to the floor, it is already on, the door opens.

• The elevator must never move while the door is open.

• Program the door with a separate state machine.

Part 4: Emergency Stop

Program the PLC elevator to stop in the event of an emergency.

• Pressing the emergency stop button PBE5 should stop the operation of the elevator.

• An indicator light ILE5 indicates the elevator has been stopped due to an emergency.

• The elevator should not respond to calls or requests.

• If an emergency occurs with the door open, the door remains open.

• The door can be opened and must remain open iff the elevator is at a floor.

• A reset pushbutton, START, must be pressed once it is safe.

• After a reset, the elevator returns to the 1st floor with the door closed.

Part 5: Four-Floor Elevator

Program the PLC elevator to service all four floors.

• The elevator can be called to Floor 4 with FPB4.

• FIL4 notifies users that the call is accepted.

• Passengers can request to go to Floor 4 with PB4.

• IL4 notifies the passenger that the request is accepted.

Part 6: Multiple Calls/Requests

Program the PLC elevator to respond to multiple simultaneous calls or requests for any floor.

• The elevator services each intermediate floor with a pending request/call that is passed.

• The elevator maintains its direction until the highest/lowest request/call is serviced.

• The elevator must never repeatedly service two floors ignoring other floors.

Part 7: Statistical Analysis

Program the PLC elevator to analyze the calls and service times of the four-floor elevator.

• Monitor the number of calls and requests made for each floor within an array.

• Monitor the average time required to service each floor within an array.

• The time to service a floor:

o Starts when the call is made

o Ends when the door closes after arriving at the floor.

• Record the date and time when the floor was serviced.

• Use an Add-On Instruction for this part.

Part 8: Fault Detection

Program the PLC to detect three unique faults.

• Each fault detected checks for something practical but conceptually different.

• Each fault generates a unique code.

• When a fault occurs, log the fault code, the system time, the system state and outputs.

• Store the information in a User-Defined Type in an array of at least 5.

• Use a periodic task for the fault detection program.

• Categorize each fault as a major or a minor fault.

• A major fault occurs if the fault array is full of minor faults.

• HMI: Elevator Operation

• HMI: Elevator Override

• HMI: Fault Display

• HMI: Statistics Display

• HMI: I/O Display

• HMI: Maintenance Mode
