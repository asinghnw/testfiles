TTP / NWs   TipTapPay  V1 Reader -  Manufacturing Test Plan
Rev4   bp. Mar 26 2020

Following data to be captured and loaded in a production database at time of manufacture.
Production database will reside on one of the test computers on the manufacturing floor.  See notes below.

Production database record: Content of record:  Notes
Date Created  Date & Time.   Source: Station 1 Computer   Internet time server source ?
Product HW Revision Hardware revision # (pcb&BOM) Drop down menu on the screen?
MSI PCB S/N MSI pcb s/n Scanned from the PCBA. 
Product S/N   Product S/N Assigned by Test Station 1, sequential numbers
K81 ID# K81 unique ID#  Read from the K81 IC.
STMicro ID  ST Micro unique ID# Read from the STM IC
Device IMEI Nordic moden IMEI#  Read from the Nordic IC 
Sim Card #  SIM Card #  Scan SN barcode from the SIM card carrier
IDTech S/N  IDTech s/n  Scan 2D barcode from IDTech label
Denomination  $5, $10, $20… other Scanned from the appropriate bin
Test a  Test results  Logged by the test computer
Test b  “ “ “     “  
Test c  “ “ “     “  
Test d  “ “ “     “  
Test e  “ “ “     “  
Date Completed  Date & Time.   Source:  Station 2 Computer  Internet time server source ?
    
    
OTHER… tbd    
    
    

 
MA ASSEMBLY AND TEST PROCESS FLOW

STATION 1
PCBA connectivity & operational test.  
Equipment:    Computer, Qty (3) Segger J-Link probes, handheld barcode reader, Label Printer.  Adhesive labels {size x, pn xxx}.     Clamshell test fixture (1-up)
  S/W:  Station1_TTP_VH_TEST_Software.   TTP_VH_SelfTest_Firmware(s).   

1.  Load PCBA onto fixture & close.
2.  Select HW Revision # from computer drop down menu
3.  Press START.
4.  Scan PCB barcode (MSI pcb SN)
5.  Computer programs Nordic, STMicro, and K81 with Self-Test Firware(s).
6.  Self-Test code tests interconnectivity with all accessible devices on the PCB, and exercises I/O.
7.  PCBA communicates Pass/Fail to the computer (connect to computer via USB or K81 UART, tbd).
8.  IF PASS:  
a.  Computer reads K81 unique ID#, STM ID# & Nordic IMEI; assigns a sequential Product S/N to the device, and opens a Database record of this [Device SN], with data:
i.  [Date Created], [HW Revision #],[MSI pcb SN], [Product SN], [STM ID#], [Device IMEI]. 
b.  Computer loads and verifies Application Firmware(s) to all programmable devices on the PCBA. 
c.  Computer indicates ‘PASS’ on the screen
d.  Computer prints ID# label w [Device SN]  {b/c and human readable}
e.  Open fixture, apply ID# label, Unload PCBA.
f.  Put the PCBA in the ‘Station 1 PASSED’ Bin 
g.  Press DONE.   Computer displays ‘Load new TTP PCBA’
9.  IF FAIL:
a.  Computer displays ‘FAIL;  Failure Code ###’
b.  Printer prints lable:  ‘FAIL {code ###}’
c.  Open fixture, apply Fail label, Unload PCBA.
d.  Put the PCBA in the ‘FAILED’ Bin 
e.  Press DONE.   Computer displays ‘Load new TTP PCBA’ 
f.  Device Failure Code is logged to the database
10. END


STATION 2
Configure PCBA + SIM + IDTech module
Equipment:    Computer, 2D b/c reader, torque-limited screwdriver, Label Printer, Adhesive Labels {size yyy pn yyy}.     Assembly Fixture (holds enclosure bottom during assembly and test.)
  S/W:  Station2_TTP_VH_TEST_Software.   

1.  Place an Enclosure Bottom into the Assembly Fixture.
2.  Press START on computer.
3.  Pick a PCBA from the Station 1 PASSED Bin, and b/c scan its [Device SN].
4.  Place PCBA on fixture pins (pins protrude through the mounting holes, and hold the PCB to aid assembly)
5.  Pick and install coin-cell battery
6.  Pick a SIM Card, b/c scan the SN# of SIM Card, and install SIM.
7.  Pick and install a flexcable onto the PCBA
8.  Lift PCBA off of the fixture pins, flip it over, and install PCBA into the Enclosure Bottom.
a.  {insertion & screw instructions…} 
9.  Pick an IDTech Reader, and b/c scan its SN#. 
10. Connect flexcable to IDTech, and install IDTech into enclosure.
11. Insert the USB cable from the computer into the product.


12. Configure IDTech and run a full functional test of the product:  
Upon application of power to the product…
a.  K81 detects IDTech is present and is not configured. (** Amrit, see note1 at bottom)
b.  K81 attempts to communicate to TTP Production Server via LTE modem over cellular network.
c.  K81 requests configuration data for the IDTech from the Server.  Server responds with data.   K81 attempts to program the IDTech.
d.  K81 indicates to the Computer if the configuration was successful or failed.
i.  IF FAIL due to communications error, 
• THEN Computer displays “LTE FAIL, Code ###”;  Goto FAIL.
ii. IF FAIL due to IDTECH failure, 
• THEN Computer displays “IDTECH FAIL, Code ###”;  Goto FAIL.
iii.  IF PASS, Then Computer displays “IDTECH CONFIGURATION SUCCESSFUL, PROCEED TO CARD TEST”.
e.  Operator presents an NFC Card to the IDTech Reader
f.  Product reads the card and attempts a transaction with the TTP Production Server:
i.  Product and Server handshake and transact…
ii. IF Server denies the payment as expected (this new device is unknown to the server)
• THEN Computer displays “CARD TEST SUCCESSFUL, APPLY DENOMINATION LABEL & SEAL”
• ELSE Computer displays “CARD TEST FAIL,  CODE ###”; Goto FAIL

g.  IF FAIL (any of the above):
i.  Computer displays ‘FAIL {Failure Code##}’
ii. Computer logs failure code to the Database record of this [Device SN]
iii.  Computer prints ‘FAIL   {Failure Code##}’ label 
iv. Operator bags the unit, applies FAIL label to the bag, puts it in the FAIL STATION2 BIN 
v.  Press DONE.     Computer Displays ‘START NEW TTP ASSEMBLY’


13. Operator Select a TTP Denomination Label from BIN X$
• Q { travel sheet? Or, do we preload the Qty of each denom in the system, and have the computer prompt and verify? }
14. Operator b/c scans BIN X$ . 
15. Install TTP Denomination Label on the enclosure or Enclosure Cover
16. Seal the Enclosure Cover.
17. Press PRINT LABELS on the computer.
18. Computer prints qty (2) Product labels. One for the product, one for the unit box
19. Computer appends [SIM Card #], [IDTech S/N], [Denomination], [Date Completed], and test results to the Database record of this [Device SN]
20. Apply Product label to the bottom of the enclosure
21. Pick: (1) Power Supply, (1) Power Cable, (1) QuickStart Sheet, and pack with the product in a Unit Box.
22. Apply 2nd label to the Unit Box.  
23. Press DONE.     Computer Displays ‘START NEW TTP ASSEMBLY’
24. Pack in Master Carton


**Note1:  Amrit, if a unit fails a test during final assembly, and is repaired, it will need to go back to Station 2 for retesting.  It is possible that this unit may have successfully configured the IDTech in its first round.  So we need a way for the computer to recognize if the IDTech has been pre-programmed and proceed anyways.

ADDITIONAL NOTES:
1.  Station 1 and Station 2 PCs to be connected via wired network: ‘TTP Production Network’.
2.  TTP Production Network to be physically separate from Microart network.
3.  Database to reside on Station 2  {Station 2 writes a backup copy to Station 1}
4.  Station 2 to have internet connection through firewall for remote access.  Station 2 will allow read-only access to the database to the remote party.   
references

Numerology  (Chris G)
 


Data from the production test database that is available to the Server Database:
(Customer Admin)  Processor Payment App
Device S/N  TID
Any other data as per Behzad’s requirements 
  
  
  
  

