# DataContainer
**Cache für deine Daten**
## Projektidee
DataContainer - Ermöglicht Daten in strukturierter Form von anderen Services zu 
speichern und für andere Services bereitstellen. Dabei kann der DataContainer Service
verschiedene Schnittstellen anbieten, wie z.B. MQTT, REST oder gPRC... Des Weiteren sind die
Daten nicht persistent gehalten sondern mit einem Ablaufdatum versehen... Im Grunde ist die Idee, 
stelle einen gemeinsamen Cache für verschiedene Teilnehmer da und 
ermögliche einen Standardisierten Zugriff auf die Daten. Der Unterschied zu NoSQL ist die 
Daten werden in gängigen Protokollen bereitgestellt und eine Persistierung der Daten ist im
Allgemeinen nicht vorgesehen

### Festlegung

Der Seperator um Gruppen zu trennen is 0x1D

### Was will ich erreichen
pub -t D01 -m UID/Destination/LOOP/ALGO 

Cache GET/D01/uid=012345678 => RESULT 012345678/11501;11201/0/PRIO

Cache GET/D01/LOOP=0  { [ 012345678/11501;11201/0/PRIO 0x1D 012345679/11501;11201/0/PRIO  ]

Cache GET/D01/ALGO=PRIO&LOOP=1 für und verknüpfung


### Data Definition
```yaml
types:
     - TEMPERATURE_SENSOR_01
     
TEMPERATURE_SENSOR_01:
  expire_in: 20min
  key: T_S01
  name: TEMPERATURE_SENSOR
  value:
    - ID: index
    - TIMESTAMP: string
    - TEMPERATURE: real
  read_event:
    - type: mqtt
    - broker: localhost:1883
    - subscribe: SENSOR/01/TEMPERATURE
    - format: "%r"
    - insert:
        - ID: ink_index(0)
        - TIMESTAMP: timestamp
        - TEMPERATURE: $1
  send_event:
    - type: mqtt
    - broker: localhost:1883
    - topic: CALCULATED
    - prepare: mean(TEMPERATURE)
    - when:
      - count(TEMPERATURE) >= 3
      - reset: true
           
``` 

#### Erläuterung
  - **expire_in:**

            Daten die im Container eingefügt werden haben für Gewöhnlich ein Verfallsdatum.
            Erlaubte Werte sind: ms,s,min,h,d
  - **expire_when:**

            Daten die im Container eingefügt werden haben für Gewöhnlich ein Verfallsdatum.
            Erlaubte Werte sind: count (>|<|>=|<=|==|!=) number oder yyyy-mm-dd hh:mm:ss + (dd hh:mm:ss)
  - **key:** 

            Mit key wird der nach außen Sichtbare Name der Tabelle angegeben
  - **name:**

            Interne Name des Typs in der YAML Datei
  - **value:**

            In diesem Block wird der eigentlich Typ spezifiziert. Im Allgemeinen gilt die Key Typ Structure. Das heißt 
            key: (index|string|real|enum|list)
    - ***index:***
    
            Ein Value der vom Typ index ist, nachdiesem kann in Konstanterzeit gesucht werden. 
            Ist kein Index angegeben so wird die erste key:Typ Paar genutzt.
    - ***string:***

            Ein Value der vom Typ string ist, darf alle Zeichen speichern die im ASCII Code erlaubt sind. 
            Seine maximale Länge wird durch das System begrenzt. 
    - ***real:***
    
            Ein Value der vom Typ real ist, dient der Repräsentation von Zahlen. Als Dezimaltrenner wird das "." 
            verwendet.



### REST API

### Funktionsweise Storage
Storage ist eine map dessen Values auf maps verweisen die wiederum Tabellen sind 

