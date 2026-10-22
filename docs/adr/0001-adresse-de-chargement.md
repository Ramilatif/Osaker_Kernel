# ADR 0001 — Adresse de chargement du noyau sous QEMU `virt`

- **Statut** : accepté
- **Date** : 2026-10-21
- **Jalon** : M0
- **Exigences liées** : EF-01, ENF-01, ENF-20

## Contexte

Avec `-kernel`, QEMU charge un ELF bare-metal en copiant ses segments à leur
adresse physique, puis saute sur son point d'entrée. L'adresse de liaison du
noyau détermine donc directement où il se retrouve en RAM.

Sur la machine `virt` :

- la RAM commence à `0x40000000` (nœud `memory` du device tree) ;
- pour une image qui n'est pas un noyau Linux, QEMU place le device tree
  **au début de la RAM** ([documentation QEMU `virt`](https://www.qemu.org/docs/master/system/arm/virt.html),
  section *Hardware configuration information for bare-metal programming*) ;
- le DTB généré par QEMU a un champ `totalsize` de `0x100000` (1 Mio). Son
  contenu utile est bien plus petit (~8 Kio une fois recompacté par `dtc`),
  mais toute la zone de 1 Mio doit être considérée comme occupée.

Le noyau ne doit pas écraser le DTB : il en a besoin pour découvrir le
matériel (EF-04, ENF-02).

## Décision

Le noyau est lié et chargé à **`0x40200000`**, soit RAM + 2 Mio.

## Justification

1. **Pas de chevauchement avec le DTB.** La zone `0x40000000`–`0x400FFFFF`
   est réservée au DTB ; le noyau commence au-delà.
2. **Alignement sur un bloc de niveau 2.** En granule 4 Kio, une table de
   pages contient 4096 / 8 = 512 descripteurs. Une entrée de niveau 3 mappe
   4 Kio, donc une entrée de niveau 2 couvre 512 × 4 Kio = 2 Mio. Un noyau
   aligné sur 2 Mio peut être mappé par blocs de niveau 2 en M2, et aucune
   région de 2 Mio n'est partagée entre le DTB et le noyau.

## Alternatives rejetées

| Adresse | Raison du rejet |
| --- | --- |
| `0x40000000` | Écrase le DTB. |
| `0x40080000` (offset 512 Kio, convention historique Linux arm64) | Chevauche le DTB de 1 Mio généré par QEMU. |
| `0x40100000` (juste après le DTB) | Fonctionne, mais n'est pas aligné sur 2 Mio : le premier bloc de niveau 2 mélangerait DTB et noyau. |

## Conséquences

- `linker.ld` fixe l'adresse de base à `0x40200000`. C'est la seule adresse
  de chargement figée du projet, conformément à ENF-20.
- La première zone de 2 Mio de la RAM n'est jamais utilisée par le noyau pour
  son image ; l'allocateur de pages (EF-08) devra la marquer réservée tant que
  le DTB est utilisé.
- Sur la STM32MP257F-DK, l'adresse est imposée par U-Boot et le DTB est
  ailleurs : cette décision est propre à `virt`. Le passage en code relogeable
  (ENF-20, M2) rendra le noyau indépendant de cette adresse.
- **QEMU ne transmet pas l'adresse du DTB à un ELF bare-metal.** Vérifié
  sous GDB au point d'entrée (QEMU 11.1, `-M virt -cpu cortex-a53`) :
  `pc = 0x40200000`, `x0 = 0x0`, PSTATE = EL1h, DAIF masqués. Le DTB est
  bien à `0x40000000`, mais le noyau n'en est pas informé, alors que U-Boot
  passera son adresse dans `x0` sur la carte. Le mode de découverte du DTB
  (adresse confinée dans `platform/qemu-virt/`, ou boot au format *arm64
  Image* pour que QEMU remplisse `x0` comme U-Boot) sera tranché par une ADR
  dédiée en M1, avant l'analyse du device tree (EF-04).
