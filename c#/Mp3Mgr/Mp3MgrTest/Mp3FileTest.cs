using Mp3Mgr;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace Mp3MgrTest
{
    
    
    /// <summary>
    ///This is a test class for Mp3FileTest and is intended
    ///to contain all Mp3FileTest Unit Tests
    ///</summary>
    [TestClass()]
    public class Mp3FileTest
    {


        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for Mp3File Constructor
        ///</summary>
        [TestMethod()]
        public void Mp3FileConstructorTest()
        {
            string fileName = "001 title.mp3";
            Mp3File target = new Mp3File(fileName);
            Assert.AreEqual(true, target.IsValid);
            Assert.AreEqual(1, target.TrackNo);

            target = new Mp3File("99 title.mp3");
            Assert.AreEqual(true, target.IsValid);
            Assert.AreEqual(99, target.TrackNo);

            target = new Mp3File("9 title.mp3");
            Assert.AreEqual(false, target.IsValid);
            Assert.AreEqual(0, target.TrackNo);

            target = new Mp3File("11 Title title, title.mp3");
            Assert.AreEqual(true, target.IsValid);
            Assert.AreEqual(11, target.TrackNo);
            Assert.AreEqual("Title title, title", target.Title);

            target = new Mp3File("11 artist - album - Title title, title.mp3");
            Assert.AreEqual(true, target.IsValid);
            Assert.AreEqual(11, target.TrackNo);
            Assert.AreEqual("Title title, title", target.Title);
            Assert.AreEqual("artist", target.OptionalArtist);
            Assert.AreEqual("album", target.OptionalAlbum);

            target = new Mp3File("11 artist - Title title, title.mp3");
            Assert.AreEqual(true, target.IsValid);
            Assert.AreEqual(11, target.TrackNo);
            Assert.AreEqual("Title title, title", target.Title);
            Assert.AreEqual("artist", target.OptionalArtist);
            Assert.AreEqual(null, target.OptionalAlbum);
        }
    }
}
