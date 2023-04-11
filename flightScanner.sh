#!/bin/bash

today=$(date +%s)
yesterday=$(date +%s --date="yesterday")


for ICAO in $@; do
    
    airportInfo=$(curl -s "https://opensky-network.org/api/airports?icao=$ICAO")

    if [ "$airportInfo" == "" ];  then
        echo "$ICAO is invalid ICAO code or does not exist"
    else
        echo "creating directory for $ICAO"
        mkdir "$ICAO"
        
        echo "creating apt and dpt files for $ICAO"
        cd ./"$ICAO"
        
        curl -s "https://opensky-network.org/api/flights/arrival?airport=$ICAO&begin=$yesterday&end=$today" | python3 -m json.tool | tr -d '[/]/{/}/,/"' | awk '{print $2}' | awk NF | xargs -n12 | sed 's/ /,/g' | sed '1i icao24,firstSeen,estDepartureAirport,lastSeen,estArrivalAirport,callsign,estDepartureAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departureAirportCandidatesCount,arrivalAirportCandidatesCount' > $ICAO.apt
            
        curl -s "https://opensky-network.org/api/flights/departure?airport=$ICAO&begin=$yesterday&end=$today" | python3 -m json.tool | tr -d '[/]/{/}/,/"' | awk '{print $2}' | awk NF | xargs -n12 | sed 's/ /,/g' | sed '1i icao24,firstSeen,estDepartureAirport,lastSeen,estArrivalAirport,callsign,estDepartureAirportHorizDistance,estDepartureAirportVertDistance,estArrivalAirportHorizDistance,estArrivalAirportVertDistance,departureAirportCandidatesCount,arrivalAirportCandidatesCount' > $ICAO.dpt

                
        cd ../
    fi
done