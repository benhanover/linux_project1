# Flight-Scanner Project


### **Submitions**:<br>
Lahav Rabinovitz: 209028349<br>
Ben Hanover: 206564981<br>
Meir Zoref: 305643231<br>

Link to the github repo(README.md file will be displayed properly)
<br>
https://github.com/benhanover/linux_project1/blob/main/README.md
<br>


## **How to run this project**
### **Step one**: add executable permissions to the scripts<br>
 Run: <pre>chmod u+x flightScanner.sh clean.sh</pre><br>
### **Step two**: load the data base<br>
Run: <pre>./flightScanner.sh (airports icao names)</pre><br>
**Example:** <pre>./flightScanner.sh EGGW EGLL LLBG EGKK</pre><br>
### **Step three**: compile the project<br>
Run:<pre>make</pre><br>
### **Step four**: test the project according to each program<br>
### Q1:<br>
Run <pre> ./arrivals.out (airport icao names)</pre><br>
**Example:** <pre>./arrivals.out EGGW EGLL LLBG EGKK</pre><br>
### Q2:<br>
Run <pre> ./full_schedule.out (airport icao name)</pre><br>
**Example:** <pre>./full_schedule.out EGGW</pre><br>
### Q3:<br>
Run <pre> ./airplane.out (aircraft icao24 names)</pre><br>
**Example:** <pre>./airplane.out 4c808e 73806a</pre><br>
### Q4:<br>
Run <pre>./updateDB.out</pre><br>
### **Step five**: clean all the ".o/.out/so" files :<br>
Run <pre>make clean</pre>
