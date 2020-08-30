-- phpMyAdmin SQL Dump
-- version 4.2.11
-- http://www.phpmyadmin.net
--
-- Client :  127.0.0.1
-- Généré le :  Dim 29 Novembre 2015 à 09:11
-- Version du serveur :  5.6.21
-- Version de PHP :  5.6.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Base de données :  `raspberry`
--
CREATE DATABASE IF NOT EXISTS `raspberry` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `raspberry`;

-- --------------------------------------------------------

--
-- Structure de la table `connection`
--

DROP TABLE IF EXISTS `connection`;
CREATE TABLE IF NOT EXISTS `connection` (
  `connection_date_time` datetime NOT NULL,
  `ip_address` varchar(100) NOT NULL,
  `browser` varchar(200) NOT NULL,
  `request_uri` varchar(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Structure de la table `disk`
--

DROP TABLE IF EXISTS `disk`;
CREATE TABLE IF NOT EXISTS `disk` (
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `disk_total` int(11) NOT NULL,
  `disk_free` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Structure de la table `memory`
--

DROP TABLE IF EXISTS `memory`;
CREATE TABLE IF NOT EXISTS `memory` (
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `mem_total` float NOT NULL,
  `mem_free` float NOT NULL,
  `swap_total` float NOT NULL,
  `swap_free` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Structure de la table `pi_command_configuration`
--

DROP TABLE IF EXISTS `pi_command_configuration`;
CREATE TABLE IF NOT EXISTS `pi_command_configuration` (
  `command` varchar(10) NOT NULL,
  `gpio` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Structure de la table `pi_schedule`
--

DROP TABLE IF EXISTS `pi_schedule`;
CREATE TABLE IF NOT EXISTS `pi_schedule` (
`id` int(11) NOT NULL,
  `creation_date` datetime NOT NULL,
  `day` tinyint(4) NOT NULL,
  `time` time DEFAULT NULL,
  `command` varchar(10) NOT NULL,
  `last_execution` datetime DEFAULT NULL,
  `next_execution` datetime DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Structure de la table `temperature`
--

DROP TABLE IF EXISTS `temperature`;
CREATE TABLE IF NOT EXISTS `temperature` (
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `temperature` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `disk`
--
ALTER TABLE `disk`
 ADD PRIMARY KEY (`creation_date`);

--
-- Index pour la table `memory`
--
ALTER TABLE `memory`
 ADD PRIMARY KEY (`creation_date`);

--
-- Index pour la table `pi_command_configuration`
--
ALTER TABLE `pi_command_configuration`
 ADD PRIMARY KEY (`command`);

--
-- Index pour la table `pi_schedule`
--
ALTER TABLE `pi_schedule`
 ADD PRIMARY KEY (`id`);

--
-- Index pour la table `temperature`
--
ALTER TABLE `temperature`
 ADD PRIMARY KEY (`creation_date`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `pi_schedule`
--
ALTER TABLE `pi_schedule`
MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=37;