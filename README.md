# Flight-Scanner Project


### **Submitions**: Lahav Rabinovitz, Ben Hanover, Meir Zoref


## **How to run this project**
### **Step one**: clone the repository<br>
 Run: <pre>git clone https://github.com/benhanover/linux_project1.git ./flight_scanner_proj</pre><br>
### **Step two**: get in to the project folder<br>
Run: <pre>cd flight_scanner_proj/</pre><br>
### **Step three**: load the data base<br>
Run: <pre>./flightScanner.sh (airports icao names)</pre><br>
**Example:** <pre>./flightScanner.sh EGGW EGLL LLBG EGKK</pre><br>
### **Step four**: compile the project<br>
Run:<pre>make</pre><br>
### **Step five**: test the project according to each program<br>
### Q1: Run <pre> ./arrivals.out (airport icao names)</pre><br>
**Example:** <pre>./arrivals.out EGGW EGLL LLBG EGKK</pre><br>
### Q2: Run <pre> ./full_schedule.out (airport icao name)</pre><br>
**Example:** <pre>./full_schedule.out EGGW</pre><br>
### Q3: Run <pre> ./airplane.out (aircraft icao24 names)</pre><br>
**Example:** <pre>./airplane.out 4c808e 73806a</pre><br>
### Q4: Run <pre>./updateDB.out</pre><br>


