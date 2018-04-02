-- MySQL dump 10.13  Distrib 5.7.17, for macos10.12 (x86_64)
--
-- Host: 127.0.0.1    Database: 207_project
-- ------------------------------------------------------
-- Server version	5.7.16

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `course` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `department` varchar(45) NOT NULL,
  `number` varchar(45) DEFAULT NULL,
  `semester` varchar(45) DEFAULT NULL,
  `year` varchar(45) DEFAULT NULL,
  `name` varchar(45) NOT NULL,
  `section` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course`
--

LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES (1,'CMPE','207','Spring','2017','NetworkProgramming','01'),(2,'CMPE','206','Spring','2016','NetworkBasic','01'),(3,'CMPE','280','Spring','2017','UI','01'),(4,'CS','281','Fall','2017','CloudComputing','02'),(5,'CS','283','Spring','2017','Virtual','02'),(6,'CMPE','202','Fall','2017','ProgrammingOverview','02'),(14,'CMPE','280','Fall','2016','UIDesign','01'),(16,'CMPE','280','Fall','2016','UIDesign','02'),(17,'aa','dd','dd','dd','dd','dd'),(18,'a','a','a','a','a','a'),(19,'a','b','a','a','a','a'),(20,'a','a','a','b','d','a');
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `course_students_rltn`
--

DROP TABLE IF EXISTS `course_students_rltn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `course_students_rltn` (
  `courseId` int(11) NOT NULL,
  `studentId` int(11) NOT NULL,
  PRIMARY KEY (`courseId`,`studentId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course_students_rltn`
--

LOCK TABLES `course_students_rltn` WRITE;
/*!40000 ALTER TABLE `course_students_rltn` DISABLE KEYS */;
INSERT INTO `course_students_rltn` VALUES (1,1),(1,2),(1,3),(1,5),(1,6),(2,1),(2,2),(2,3),(3,1),(3,2),(3,3),(3,5),(4,2),(4,3),(4,4),(5,2),(6,1);
/*!40000 ALTER TABLE `course_students_rltn` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filestore`
--

DROP TABLE IF EXISTS `filestore`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filestore` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(256) NOT NULL,
  `filepath` varchar(256) NOT NULL,
  `courseId` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `filename_courseId` (`courseId`,`filename`),
  CONSTRAINT `filestore_ibfk_1` FOREIGN KEY (`courseId`) REFERENCES `course` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `filestore`
--

LOCK TABLES `filestore` WRITE;
/*!40000 ALTER TABLE `filestore` DISABLE KEYS */;
INSERT INTO `filestore` VALUES (5,'test1.txt','/Users/bing/Desktop/courses/CMPE207_NetworkProgrammingAndApplication/project/bitbucket/serverfiles/3/test1.txt',3),(6,'test2.txt','/Users/bing/Desktop/courses/CMPE207_NetworkProgrammingAndApplication/project/bitbucket/serverfiles/3/test2.txt',3),(10,'test3.txt','/Users/bing/Desktop/courses/CMPE207_NetworkProgrammingAndApplication/project/bitbucket/serverfiles/3/test3.txt',3),(12,'test5.txt','/Users/bing/Desktop/courses/CMPE207_NetworkProgrammingAndApplication/project/bitbucket/serverfiles/3/test5.txt',3);
/*!40000 ALTER TABLE `filestore` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person`
--

DROP TABLE IF EXISTS `person`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `first_name` varchar(45) NOT NULL,
  `last_name` varchar(45) NOT NULL,
  `email` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `gender` tinyint(1) NOT NULL,
  `role` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `email_UNIQUE` (`email`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person`
--

LOCK TABLES `person` WRITE;
/*!40000 ALTER TABLE `person` DISABLE KEYS */;
INSERT INTO `person` VALUES (1,'BingStu','Tran','bing@gmail.com','3234242',1,2),(2,'LamPro','Shi','Alm@gmail.com','4324324',1,1),(3,'BingStu2','Shi','bingshi011@gmail.com','900112',0,2),(4,'BingProf','Shi','bingshi112@gmail.com','900112',0,1),(5,'LamStu2','Shi','LamP@gmail.com','4324',1,2),(6,'BingStu3','Shi','bingshi66011@gmail.com','900112',0,2),(14,'adimFirst','adminLast','admin','admin',0,0),(17,'Lam','Tran','lamtran@gmail.com','910209',2,1),(19,'Bing','Tran','bingshi0112@gmail.com','910209',2,2),(20,'a','a','a','d',1,1),(21,'bing','bing','bing','bing',1,2),(22,'lam','lam','ll','ll',0,2);
/*!40000 ALTER TABLE `person` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-05 22:04:22
