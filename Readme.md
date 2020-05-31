# Introducing Electronic Circuits and Hydrological Models to Postsecondary Physical Geography Students: Systems Science, Circuit Theory, Construction and Calibration
## by Nicholas J. Kinar
*Journal of Geoscience Communication (GC)*

## What is it?

* The download contains directories providing circuit board design files and feedback data.
Files are provided "as is" and should be checked before use in a specific application.

* Please see the associated paper for additional information.  

* Cost and part numbers in the Excel bill of material (BOM) spreadsheets are current at the time of
writing.  The cost is in Canadian dollars.  Most part numbers for passive components (resistors and capacitors)
can be substituted.  The BOM does not include materials used for kit packaging: ESD bags, plastic bags and
ESD foam used to protect IC pins.

* Bags, labelling machines and labels can be sourced from Uline (or a similar supplier):
<https://www.uline.ca/> or
<https://www.uline.com/>

* At the time of writing, an educational license is available from Altium and
the DFM Now! software can be downloaded as a no-cost license.  See the table
below for further details.

* All circuit board designs are licensed under the open hardware CERN OHL license:
http://cern.ch/cern-ohl

* All source code is provided under the GNU GPL v3:
https://www.gnu.org/licenses/gpl-3.0.en.html


## FILES

Bolded entries in the table indicate parent directories.  Non-bolded
entries indicate subdirectories.

| Directory   |     Description  |
|----------|:-------------:|
| **class-sensor-pcb-design-files** | Parent directory with design files for the class RH/temperature/pyranometer circuit|
| altium |   Design files in Altium designer format (https://www.altium.com/)  |  
| bom | Bill of Materials (Excel) |
|code	| Code for Atmel AVR microcontroller (Atmel Studio)|
|gerber	| Gerber files |
|schematic	| Schematic (PDF) |
|**feedback data**	| Directory with in-class feedback|
|**feedback-data-sleq**	| Feedback data from the SLEQ survey|
|**water-sensor-pcb-design-files**	| Directory with design files for the class water detection circuit|
|altium	|						Design files in Altium designer format (https://www.altium.com/)|
|bom |								Bill of Materials (Excel)|
|gerber |							Gerber files|
|schematic |						Schematic (PDF)|


## GERBER FILES README AND MAPPING

1.	These are 2 layer PCBs.
2.	Round up drill size to the next available drill (free or non-free) if required.
3.	Soldermask on both sides of the PCB
4.	Silkscreen on both sides of the PCB
5.	DFM Now! software can be used for Gerber preview (https://www.numericalinnovations.com/)
6.	gerbv also works well as Gerber preview software (http://gerbv.sourceforge.net/)


| Layer Extension   |     Layer Description  |
|----------|:-------------:|
|.GTO|                Top Overlay |                            
|.GTS|                Top Solder  |                            
|.GTL|                Top Layer    |                           
|.GBL|                Bottom Layer      |                      
|.GBS|                Bottom Solder   |                        
|.GM1|                Mechanical 1   |                         
|.GPT|                Top Pad Master      |                    
|.GPB|                Bottom Pad Master|
|.DRR| 				NC Drill File Report|
|.TXT|				NC Drill File|
