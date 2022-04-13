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
`
types:
     - SORT_DEST
type:
   name: SORT_DEST
   KEY: D01
   VALUES:
     - UID: STRING 
     - DESTINATIONS: 
           LIST:
                TYPE: INT 
                DELIMITER: ';'
     - LOOP:
           TYPE: INT 
     - ALGO:
         ENUM:
            - PRIO
            - ANY
    expire_in: 20min 
           
`


### REST API

### Funktionsweise Storage


Storage ist eine map dessen Values auf maps verweisen die wiederum Tabellen sind 