-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Client :  127.0.0.1
-- Généré le :  Sam 04 Juin 2016 à 00:01
-- Version du serveur :  10.1.13-MariaDB
-- Version de PHP :  7.0.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Base de données :  `raspberry`
--

-- --------------------------------------------------------

--
-- Structure de la table `pi_schedule`
--

DROP TABLE IF EXISTS `pi_schedule`;
CREATE TABLE `pi_schedule` (
  `id` int(11) NOT NULL,
  `creation_date` datetime NOT NULL,
  `day` tinyint(4) NOT NULL,
  `time` time DEFAULT NULL,
  `command` varchar(10) NOT NULL,
  `last_execution` datetime DEFAULT NULL,
  `next_execution` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `pi_schedule`
--
ALTER TABLE `pi_schedule`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `day` (`day`,`time`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `pi_schedule`
--
ALTER TABLE `pi_schedule`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=77;