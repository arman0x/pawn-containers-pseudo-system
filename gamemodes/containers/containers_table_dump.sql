-- MySQL dump 10.13  Distrib 5.6.51, for Win64 (x86_64)
--
-- Host: localhost    Database: base
-- ------------------------------------------------------
-- Server version	5.6.51

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `base`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `base` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */;

USE `base`;

--
-- Table structure for table `container_auction_players`
--

DROP TABLE IF EXISTS `container_auction_players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `container_auction_players` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `player_name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `container_id` tinyint(3) unsigned NOT NULL,
  `total_bid` int(10) unsigned NOT NULL,
  `is_actual_auction` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `won_prize_id` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Auction_Players_Container` (`container_id`) USING BTREE,
  KEY `FK_Auction_Players_Won_Item` (`won_prize_id`),
  CONSTRAINT `FK_Auction_Players` FOREIGN KEY (`container_id`) REFERENCES `containers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_Auction_Players_Won_Item` FOREIGN KEY (`won_prize_id`) REFERENCES `container_items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `container_auction_players`
--

LOCK TABLES `container_auction_players` WRITE;
/*!40000 ALTER TABLE `container_auction_players` DISABLE KEYS */;
/*!40000 ALTER TABLE `container_auction_players` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `container_items`
--

DROP TABLE IF EXISTS `container_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `container_items` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `container_type` tinyint(3) unsigned DEFAULT NULL,
  `name` varchar(32) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `chance` float DEFAULT NULL,
  `object_model` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Container_Type` (`container_type`),
  CONSTRAINT `FK_Container_Type` FOREIGN KEY (`container_type`) REFERENCES `container_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `container_items`
--

LOCK TABLES `container_items` WRITE;
/*!40000 ALTER TABLE `container_items` DISABLE KEYS */;
INSERT INTO `container_items` VALUES (1,1,'white mask',25,19036),(2,1,'green mask',20,19038),(3,1,'red mask',79,19037),(4,2,'Infernus',5,411);
/*!40000 ALTER TABLE `container_items` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`RDS`@`%`*/ /*!50003 TRIGGER trigger_containers_items_chance_limiter
BEFORE INSERT on container_items
FOR EACH ROW
BEGIN
    IF (SELECT SUM(chance) FROM container_items) + NEW.chance > 100.0 THEN
    	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Summary cannot be more then 100';
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `container_types`
--

DROP TABLE IF EXISTS `container_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `container_types` (
  `id` tinyint(3) unsigned NOT NULL,
  `description` varchar(12) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `container_types`
--

LOCK TABLES `container_types` WRITE;
/*!40000 ALTER TABLE `container_types` DISABLE KEYS */;
INSERT INTO `container_types` VALUES (1,'accessories'),(2,'cars'),(3,'houses');
/*!40000 ALTER TABLE `container_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `containers`
--

DROP TABLE IF EXISTS `containers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `containers` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `type` tinyint(3) unsigned NOT NULL,
  `start_price` tinyint(3) unsigned NOT NULL,
  `object_model` int(10) unsigned NOT NULL DEFAULT '2935',
  `object_coord_x` float DEFAULT NULL,
  `object_coord_y` float DEFAULT NULL,
  `object_coord_z` float DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Container_Type_2` (`type`),
  CONSTRAINT `FK_Container_Type_2` FOREIGN KEY (`type`) REFERENCES `container_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `containers`
--

LOCK TABLES `containers` WRITE;
/*!40000 ALTER TABLE `containers` DISABLE KEYS */;
INSERT INTO `containers` VALUES (1,1,100,2935,1082.8,-1770.86,13.8086),(2,2,150,2935,1069.17,-1770.68,13.8086),(3,3,200,2935,1089.56,-1770.99,13.8086);
/*!40000 ALTER TABLE `containers` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-08 21:57:33
