# ADR 0003 — Contrat de plateforme : `include/platform.h`

- **Statut** : accepté
- **Date** : 2026-10-26
- **Jalon** : M0
- **Exigences liées** : ENF-01, ENF-02, EF-02

## Contexte

ENF-01 interdit toute adresse matérielle ou détail de SoC en dehors de
`arch/` et `platform/`. Or le cœur du noyau (`core/`) a besoin de services
qui dépendent de la carte, à commencer par l'écriture d'un caractère sur la
console de démarrage (EF-02) : un PL011 sous QEMU `virt`, une USART de ST sur
la STM32MP257F-DK.

Il faut une frontière qui permette à `core/` d'utiliser ces services sans
connaître le matériel, et qui dise clairement quoi écrire lors d'un portage.

## Décision

- **`include/platform.h` est le contrat** : il déclare les fonctions que
  chaque plateforme doit fournir, préfixées `platform_`. Il ne contient
  aucune adresse ni aucun type propre à un composant.
- **Chaque dossier `platform/<carte>/` implémente ce contrat**, un fichier
  par composant, nommé d'après le composant (`platform/qemu-virt/pl011.c`).
  Les constantes et fonctions internes y sont `static`.
- **Le système de build choisit la plateforme** en compilant un seul dossier
  `platform/<carte>/`. Le code C ne contient aucun `#ifdef` de plateforme.
- **Le contrat reste minimal** : seule la partie dépendante du matériel y
  entre. Ce qui se construit par-dessus (chaînes, `printk`) vit dans `core/`.

Première fonction du contrat : `void platform_putc(char c)`, qui écrit un
caractère en attente active.

## Alternatives rejetées

| Option | Raison du rejet |
| --- | --- |
| `#ifdef QEMU` / `#ifdef STM32MP25` dans `core/` | Mélange matériel et cœur, contraire à ENF-01 ; chaque nouvelle carte touche tout le code. |
| Fonctions nommées d'après le composant (`pl011_putc`) appelées depuis `core/` | Le nom expose le matériel : `core/` dépendrait de QEMU. |
| Table de pointeurs de fonctions remplie au démarrage | Utile si plusieurs consoles coexistent à l'exécution ; inutile tant qu'une image cible une seule carte. À reconsidérer avec les drivers instanciés depuis le device tree (EF-15). |
| Écriture de chaînes (`platform_puts`) dans le contrat | Code identique sur toutes les cartes, dupliqué à chaque portage. |

## Conséquences

- Porter le noyau sur une carte revient à implémenter les fonctions de
  `platform.h` dans un nouveau dossier `platform/<carte>/` (M4).
- Les adresses restent codées en dur dans `platform/` jusqu'à l'analyse du
  device tree (EF-04, M1), qui pourra les remplacer.
- Les accès MMIO convertissent directement l'adresse physique en pointeur :
  valable MMU coupée uniquement. Le mapping des périphériques sera ajouté en M2.
- L'UART est supposée déjà configurée (QEMU au démarrage, U-Boot sur la carte) :
  aucune initialisation n'est faite par le noyau pour l'instant.
