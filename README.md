# C Network Simulation

🇫🇷  
Projet de simulation réseau réalisé en C dans le cadre d'un projet d'études.  
L’objectif était de représenter un réseau à partir d’un fichier de configuration, de mettre en place les structures nécessaires à son fonctionnement, et de préparer une simulation du protocole Ethernet ainsi qu’une future extension vers STP.

🇬🇧  
Network simulation project written in C for a study project.  
The goal was to represent a network from a configuration file, set up the required data structures, and prepare an Ethernet protocol simulation with a possible extension to STP.

## Présentation

🇫🇷  
Le projet consiste à modéliser un réseau composé de différents équipements (switches, hôtes, liens), selon une configuration chargée depuis un fichier externe.  
Il vise à poser les bases d’une simulation où des messages pourraient transiter entre équipements, selon les règles du protocole Ethernet.

 🇬🇧  
The project models a network composed of various devices (switches, hosts, links), based on a configuration loaded from an external file.  
It lays the groundwork for a future simulation where messages could transit between devices following Ethernet protocol rules.

## Fonctionnalités / Features

### 🇫🇷 Implémenté
- création des structures de données représentant le réseau  
- gestion des équipements (switches, hôtes, liens)  
- chargement complet du fichier de configuration  
- représentation correcte de l’état initial du réseau (t = 0)  

### 🇬🇧 Implemented
- creation of all data structures representing the network  
- device handling (switches, hosts, links)  
- full configuration file loading  
- correct representation of the network initial state (t = 0)

### Ce qui n’est pas implémenté / Not implemented

🇫🇷  
- simulation du protocole Ethernet  
- propagation et stockage des messages  
- mécanisme d’apprentissage des tables de commutation  
- protocole STP (Spanning Tree Protocol)

Note : la table de commutation est vide à l’état initial, ce qui est normal pour le temps t = 0.

🇬🇧  
- Ethernet protocol simulation  
- message propagation and storage  
- switch learning / forwarding behavior  
- STP (Spanning Tree Protocol)

Note: switching tables are empty at t = 0, which is the expected initial state.

## Objectifs du projet

🇫🇷  
- représenter un réseau à partir d’un fichier  
- préparer une simulation de messages entre équipements  
- poser les fondations d’une simulation Ethernet et STP

🇬🇧  
- represent a network from a configuration file  
- prepare message transmission simulation  
- lay the foundations for Ethernet and STP simulation

## Membres du groupe / Group members

- [Omar Farouk LASFAR](https://github.com/olasfar)
- [Muhammed Emin ERDAL](https://github.com/ygr671)
- [Ramzan ABDOULAEV](https://github.com/BorZa-45)

## Notes

🇫🇷  
Projet non terminé : seules les structures de base et le chargement du réseau ont été réalisés.

🇬🇧  
Project incomplete: only the core structures and configuration loading are implemented.

