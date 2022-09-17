/*
 *
 * NestViewer applet
 *
 * Description:
 *
 * View a recording of the nesting birds program.  Viewer source data,
 * produced by the nesting birds program when compiled with the VIEWER
 * flag, is obtained from a source file or URL. Images of the response-driving
 * portions of the birds' neural networks created during the run are also
 * available for viewing. These are obtained by compiling with the
 * ACTIVITY_TRACKING flag.
 *
 * Note: nesting birds program output must be filtered prior to use:
 *
 * nest -i <iterations> | grep ^%%__Viewer: | cut -d":" -f2 > nestsource.dat
 *
 * Usage:
 *
 * <applet code="NestViewer.class" width=<width> height=<height>>
 * [<param name=Source value="<file|URL>">] (defaults to nestsource.dat)
 * </applet>
 *
 * Animation images:
 *
 * cactus.gif
 * egg.gif
 * female.gif
 * male.gif
 * mouse.gif
 * stone.gif
 * tree.gif
 *
 * Network images:
 *
 * male*.jpg
 * female*.jpg
 *
 */

package nest.java;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.applet.Applet;
import javax.swing.*;
import javax.swing.event.*;
import javax.imageio.*;

// World cell.
class Cell
{
   int locale;
   int object;
}

// Bird.
class Bird
{
   Point location;
   int   wantsFood;
   int   wantsStone;
   int   hasObject;
   int   food;
   int[] needs;
   int response;
}

// Main applet.
public class NestViewer extends Applet implements Runnable
{
   // Parameters:

   // Milliseconds between responses.
   static final int MIN_RESPONSE_DELAY = 100;
   static final int MAX_RESPONSE_DELAY = 1000;

   // Milliseconds between display updates.
   static final int DISPLAY_UPDATE_DELAY = 50;

   // Default source name.
   static final String DEFAULT_SOURCE = "nestsource.dat";

   // World image dimensions.
   static final Dimension SCREEN_SIZE       = new Dimension(652, 600);
   static final Dimension CANVAS_SIZE       = new Dimension(850, 1100);
   static final Dimension STATUS_PANEL_SIZE = new Dimension(652, 100);
   static final Dimension CELL_SIZE         = new Dimension(30, 30);
   static final Dimension LOCALE_SIZE       = new Dimension(15, 15);
   static final Dimension OBJECT_SIZE       = new Dimension(20, 20);
   static final Dimension SMALL_BIRD_SIZE   = new Dimension(15, 15);
   static final Dimension BIRD_SIZE         = new Dimension(80, 80);

   // Definitions.

   // Genders:
   static final int MALE   = 0;
   static final int FEMALE = 1;

   // Responses:
   static final int DO_NOTHING       = 0;
   static final int EAT              = 1;
   static final int GET              = 2;
   static final int GO_TO_DESERT     = 3;
   static final int GO_TO_FOREST     = 4;
   static final int GO_TO_MATE       = 5;
   static final int LAY_EGG          = 6;
   static final int LOOK_FOR_FOOD    = 7;
   static final int LOOK_FOR_STONE   = 8;
   static final int PUT              = 9;
   static final int RECEIVE          = 10;
   static final int STEP             = 11;
   static final int TOSS             = 12;
   static final int TURN             = 13;
   static final int WANT_STONE_SET   = 14;
   static final int WANT_STONE_UNSET = 15;

   // Locales:
   static final int DESERT    = 0;
   static final int FOREST    = 1;
   static final int GRASSLAND = 2;

   // Objects:
   static final int NO_OBJECT = 0;
   static final int EGG       = 1;
   static final int FOOD      = 2;
   static final int STONE     = 3;

   // World.
   Dimension worldSize;
   Cell[][] world;
   boolean worldCreated = false;

   // Birds.
   Bird birds[];
   int  responseNumber[];

   // Screen size.
   Dimension screenSize;

   // Display (buffered).
   ScrollPane canvasScroll;
   Dimension  canvasScrollSize;
   Canvas     canvas;
   Graphics   canvasGraphics;
   Image      worldImage;
   Graphics   worldImageGraphics;
   Dimension  worldImageSize;
   Point      femaleStatusLocation;
   Point      femaleImageLocation;
   Point      femaleObjectLocation;
   Point      maleStatusLocation;
   Point      maleImageLocation;
   Point      maleObjectLocation;
   Point      statusInfoOffset;
   Point      localeImageOffset;
   Point      objectImageOffset;
   Point      femaleImageOffset;
   Point      maleImageOffset;

   // Source data.
   String         source;
   BufferedReader sourceReader;

   // Threads.
   Thread worldThread;
   Thread displayThread;

   // Control panel.
   Panel     controlPanel;
   Dimension controlPanelSize;
   Choice    viewChoice;
   Checkbox  stepButton;
   JSlider   responseSlider;
   int       response = MAX_RESPONSE_DELAY;

   // View choices.
   static final int WORLD      = 0;
   static final int MALE_NET   = 1;
   static final int FEMALE_NET = 2;
   static final int README     = 3;

   // Images
   Image maleImage;
   Image maleSmallImage;
   Image femaleImage;
   Image femaleSmallImage;
   Image treeImage;
   Image cactusImage;
   Image mouseImage;
   Image stoneImage;
   Image eggImage;
   Image maleNetImage;
   Image femaleNetImage;

   // Bird network image loaded.
   int maleNetImageLoaded   = -1;
   int femaleNetImageLoaded = -1;

   // Font.
   Font        font = new Font("Helvetica", Font.BOLD, 12);
   FontMetrics fontMetrics;
   int         fontAscent;
   int         fontWidth;
   int         fontHeight;

   // Readme.
   String[] readme =
   {
      "View an animation of the nesting birds program.",
      "The male and female birds cooperate to build a nest",
      "The female builds the nest and lays and egg in it.",
      "The male forages for food (mice) in a forest and",
      "nesting material (stones) in a desert.",
      "",
      "Each bird is controlled by a separate goal-seeking",
      "neural network. They cooperate by observing cues from",
      "each other. The response-driving portion of each",
      "network can be viewed at any step.",
      "",
      "Neural network legend:",
      "triangle: input neuron (receptor)",
      "inverted triangle: output neuron (motor)",
      "circle: enabling (excitatory) mediator neuron",
      "box: disabling (inhibitory) mediator neuron",
      "double outline: firing neuron",
      "shaded shape: goal neuron",
      "",
      "Mediator neurons connect cause neurons to effect neurons.",
      "An enabled mediator allows a firing cause neuron to",
      "alter the enablement of the effect neuron, either by",
      "enabling it or disabling it, depending on the mediator type.",
      "",
      "Neurons that are associated with need-reduction are goals.",
      "Motive back-propagates from goal neurons through the network to",
      "motor neurons to fire them, thus producing responses to the",
      "environment. Motivating an enabled mediator influences its",
      "cause and effect neurons to fire in sequence. Motivating a",
      "disabled mediator influences the mediator's super-mediators",
      "to enable it.",
      "",
      "Reference: tom.portegys.com/research.html#mona2"
   };

   // Applet information.
   public String getAppletInfo()
   {
      return("Nesting birds viewer by Tom Portegys (Version 1.1, 2004)");
   }


   // Initialize.
   public void init()
   {
      URL          u;
      Image        image;
      MediaTracker tracker;

      // Open viewer source.
      if ((source == null) && ((source = getParameter("Source")) == null))
      {
         source = DEFAULT_SOURCE;
      }
      try
      {
         sourceReader = new BufferedReader(new FileReader(source));
      }
      catch (Exception e)
      {
         sourceReader = null;
      }
      if (sourceReader == null)
      {
         u = null;
         try
         {
            u = new URL(source);
         }
         catch (MalformedURLException e)
         {
            try
            {
               u = new URL(getCodeBase(), source);
            }
            catch (Exception ex)
            {
               u = null;
            }
         }
         if (u != null)
         {
            try
            {
               sourceReader = new BufferedReader(new InputStreamReader(u.openStream()));
               status("");
            }
            catch (Exception e) {
               status("Error opening " + source + ": " + e.getMessage());
               System.exit(1);
            }
         }
         else
         {
            status("Cannot access " + source);
            System.exit(1);
         }
      }

      // Create birds.
      birds                  = new Bird[2];
      birds[MALE]            = new Bird();
      birds[MALE].location   = new Point();
      birds[MALE].needs      = new int[4];
      birds[FEMALE]          = new Bird();
      birds[FEMALE].location = new Point();
      birds[FEMALE].needs    = new int[2];
      responseNumber         = new int[2];
      responseNumber[MALE]   = 0;
      responseNumber[FEMALE] = 0;

      // Create display.
      screenSize = SCREEN_SIZE;
      setSize(screenSize);
      setLayout(new BorderLayout());
      canvasScroll     = new ScrollPane();
      canvasScrollSize = new Dimension(screenSize.width, (int)((double)screenSize.height * .95));
      canvasScroll.setBounds(0, 0, canvasScrollSize.width, canvasScrollSize.height);
      canvas = new Canvas();
      canvas.setBounds(0, 0, CANVAS_SIZE.width, CANVAS_SIZE.height);
      add(canvasScroll, BorderLayout.NORTH);
      canvasScroll.add(canvas, null);
      canvasGraphics = canvas.getGraphics();

      // Create control panel.
      controlPanel     = new Panel();
      controlPanelSize = new Dimension(screenSize.width,
                                       (int)((double)screenSize.height * .05));
      controlPanel.setBounds(0, canvasScrollSize.height,
                             controlPanelSize.width, controlPanelSize.height);
      add(controlPanel);
      controlPanel.add(new Label("View:", Label.RIGHT));
      viewChoice = new Choice();
      viewChoice.addItem("World");
      viewChoice.addItem("Male net");
      viewChoice.addItem("Female net");
      viewChoice.addItem("Readme");
      viewChoice.addItemListener(new viewChoiceItemListener());
      controlPanel.add(viewChoice);
      stepButton = new Checkbox("Step");
      controlPanel.add(stepButton);
      controlPanel.add(new Label("Fast", Label.RIGHT));
      responseSlider = new JSlider(Scrollbar.HORIZONTAL, MIN_RESPONSE_DELAY,
                                   MAX_RESPONSE_DELAY, MAX_RESPONSE_DELAY);
      responseSlider.addChangeListener(new responseSliderListener());
      controlPanel.add(responseSlider);
      controlPanel.add(new Label("Stop", Label.LEFT));

      // Get images.
      tracker = new MediaTracker(this);
      image   = loadImage("male.gif");
      tracker.addImage(image, 0);
      maleImage = image.getScaledInstance(BIRD_SIZE.width,
                                          BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleImage, 0);
      maleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                               SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleSmallImage, 0);
      image = loadImage("female.gif");
      tracker.addImage(image, 0);
      femaleImage = image.getScaledInstance(BIRD_SIZE.width,
                                            BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleImage, 0);
      femaleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                                 SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleSmallImage, 0);
      image = loadImage("tree.gif");
      tracker.addImage(image, 0);
      treeImage = image.getScaledInstance(LOCALE_SIZE.width,
                                          LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(treeImage, 0);
      image = loadImage("cactus.gif");
      tracker.addImage(image, 0);
      cactusImage = image.getScaledInstance(LOCALE_SIZE.width,
                                            LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(cactusImage, 0);
      image = loadImage("mouse.gif");
      tracker.addImage(image, 0);
      mouseImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(mouseImage, 0);
      image = loadImage("stone.gif");
      tracker.addImage(image, 0);
      stoneImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(stoneImage, 0);
      image = loadImage("egg.gif");
      tracker.addImage(image, 0);
      eggImage = image.getScaledInstance(OBJECT_SIZE.width,
                                         OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(eggImage, 0);
      try
      {
         tracker.waitForAll();
      }
      catch (InterruptedException e) {
         status("Image loading interrupted");
         System.exit(1);
      }

      // Set font data.
      Graphics g = getGraphics();
      g.setFont(font);
      fontMetrics = g.getFontMetrics();
      fontAscent  = fontMetrics.getMaxAscent();
      fontWidth   = fontMetrics.getMaxAdvance();
      fontHeight  = fontMetrics.getHeight();
   }


   // Start.
   public void start()
   {
      // Create world update thread.
      if (worldThread == null)
      {
         worldThread = new Thread(this);
         worldThread.start();
      }

      // Create canvas update thread.
      if (displayThread == null)
      {
         displayThread = new Thread(this);
         displayThread.start();
      }
   }


   // Stop.
   public void stop()
   {
      if (worldThread != null)
      {
         worldThread = null;
      }

      if (displayThread != null)
      {
         displayThread = null;
      }
   }


   // Run.
   public void run()
   {
      long t;

      // Lower this thread's priority and get the current time.
      Thread.currentThread().setPriority(Thread.MIN_PRIORITY);
      t = System.currentTimeMillis();

      // Initialize world.
      if (Thread.currentThread() == worldThread) { updateWorld(); }

      // World update loop.
      while (Thread.currentThread() == worldThread)
      {
         if (viewChoice.getSelectedIndex() == WORLD)
         {
            if ((response < MAX_RESPONSE_DELAY) || (stepButton.getState() == true))
            {
               updateWorld();
            }

            if (stepButton.getState() == true)
            {
               stepButton.setState(false);
               t = System.currentTimeMillis();
               continue;
            }
         }

         // Set the timer for the next loop.
         try
         {
            t += response;
            Thread.sleep(Math.max(0, t - System.currentTimeMillis()));
         }
         catch (InterruptedException e) { break; }
      }

      // Display update loop.
      while (Thread.currentThread() == displayThread)
      {
         updateDisplay();

         try
         {
            Thread.sleep(DISPLAY_UPDATE_DELAY);
         }
         catch (InterruptedException e) { break; }
      }
   }


   // Paint.
   public void paint(Graphics g)
   {
      updateDisplay();
      super.paint(g);
   }


   // Update world - return after EOF or break;
   public void updateWorld()
   {
      String          l, s;
      StringTokenizer t;
      int             x, y, g;

      if (sourceReader == null) { return; }

      // Read source and update world.
      l = "";
      try
      {
         while ((l = sourceReader.readLine()) != null)
         {
            t = new StringTokenizer(l, "[].=,");
            s = t.nextToken();
            if (s.equals("break"))
            {
               return;
            }
            else if (s.equals("status"))          // status
            {
               s = t.nextToken();
               status(s + "");
               return;
            }
            else if (s.equals("world"))          // world change
            {
               s = t.nextToken();
               if (s.equals("dimensions"))
               {
                  // Set world dimensions.
                  s                = t.nextToken();
                  worldSize        = new Dimension();
                  worldSize.width  = Integer.parseInt(s);
                  s                = t.nextToken();
                  worldSize.height = Integer.parseInt(s);
                  world            = new Cell[worldSize.width][worldSize.height];
                  for (x = 0; x < worldSize.width; x++)
                  {
                     for (y = 0; y < worldSize.width; y++)
                     {
                        world[x][y]        = new Cell();
                        world[x][y].locale = GRASSLAND;
                        world[x][y].object = NO_OBJECT;
                     }
                  }
                  birds[MALE].location.x   = 0;
                  birds[MALE].location.y   = 0;
                  birds[MALE].wantsFood    = 0;
                  birds[MALE].wantsStone   = 0;
                  birds[MALE].hasObject    = NO_OBJECT;
                  birds[MALE].food         = 0;
                  birds[MALE].needs[0]     = 0;
                  birds[MALE].needs[1]     = 0;
                  birds[MALE].needs[2]     = 0;
                  birds[MALE].needs[3]     = 0;
                  birds[MALE].response     = DO_NOTHING;
                  birds[FEMALE].location.x = 0;
                  birds[FEMALE].location.y = 0;
                  birds[FEMALE].wantsFood  = 0;
                  birds[FEMALE].wantsStone = 0;
                  birds[FEMALE].hasObject  = NO_OBJECT;
                  birds[FEMALE].food       = 0;
                  birds[FEMALE].needs[0]   = 0;
                  birds[FEMALE].needs[1]   = 0;
                  birds[FEMALE].response   = DO_NOTHING;

                  // Initialize graphics.
                  if ((x = worldSize.width * CELL_SIZE.width) < STATUS_PANEL_SIZE.width)
                  {
                     x = STATUS_PANEL_SIZE.width;
                  }
                  x++;
                  y = (worldSize.height * CELL_SIZE.height) + STATUS_PANEL_SIZE.height;
                  worldImageSize     = new Dimension(x, y);
                  worldImage         = createImage(x, y);
                  worldImageGraphics = worldImage.getGraphics();
                  y = 1;
                  femaleStatusLocation = new Point(0, y);
                  x = (int)((STATUS_PANEL_SIZE.width - (2.5 * BIRD_SIZE.width)) / 2);
                  femaleImageLocation = new Point(x, y);
                  x = femaleImageLocation.x + ((BIRD_SIZE.width * 7) / 8);
                  y = femaleImageLocation.y + (BIRD_SIZE.height / 8);
                  femaleObjectLocation = new Point(x, y);
                  x = 1;
                  y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
                  femaleImageOffset = new Point(x, y);
                  x = (int)(femaleImageLocation.x + (1.5 * BIRD_SIZE.width));
                  y = 1;
                  maleImageLocation = new Point(x, y);
                  x = maleImageLocation.x + BIRD_SIZE.width;
                  maleStatusLocation = new Point(x, y);
                  x = maleImageLocation.x - (BIRD_SIZE.width / 8) - 1;
                  y = maleImageLocation.y + (BIRD_SIZE.height / 8);
                  maleObjectLocation = new Point(x, y);
                  x = CELL_SIZE.width - SMALL_BIRD_SIZE.width;
                  y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
                  maleImageOffset = new Point(x, y);
                  x = 2;
                  y = 2 + fontAscent;
                  statusInfoOffset  = new Point(x, y);
                  localeImageOffset = new Point(1, 1);
                  x = (CELL_SIZE.width - OBJECT_SIZE.width) / 2;
                  y = (CELL_SIZE.height - OBJECT_SIZE.height) / 2;
                  objectImageOffset = new Point(x, y);
                  worldCreated      = true;
               }
               else
               {
                  // Set locale or object value.
                  x = Integer.parseInt(s);
                  s = t.nextToken();
                  y = Integer.parseInt(s);
                  s = t.nextToken();
                  if (s.equals("locale"))
                  {
                     s = t.nextToken();
                     world[x][y].locale = Integer.parseInt(s);
                  }
                  else if (s.equals("object"))
                  {
                     s = t.nextToken();
                     world[x][y].object = Integer.parseInt(s);
                  }
                  else
                  {
                     throw(new IOException());
                  }
               }
            }
            else            // bird condition

            {
               if (s.equals("male"))
               {
                  g = MALE;
               }
               else if (s.equals("female"))
               {
                  g = FEMALE;
               }
               else
               {
                  throw(new IOException());
               }
               s = t.nextToken();
               if (s.equals("location"))
               {
                  s = t.nextToken();
                  x = Integer.parseInt(s);
                  s = t.nextToken();
                  y = Integer.parseInt(s);
                  birds[g].location.x = x;
                  birds[g].location.y = y;
               }
               else if (s.equals("condition"))
               {
                  s = t.nextToken();
                  birds[g].wantsFood = Integer.parseInt(s);
                  s = t.nextToken();
                  birds[g].wantsStone = Integer.parseInt(s);
                  s = t.nextToken();
                  birds[g].hasObject = Integer.parseInt(s);
               }
               else if (s.equals("food"))
               {
                  s             = t.nextToken();
                  birds[g].food = Integer.parseInt(s);
               }
               else if (s.equals("needs"))
               {
                  if (g == MALE)
                  {
                     s = t.nextToken();
                     birds[g].needs[0] = Integer.parseInt(s);
                     s = t.nextToken();
                     birds[g].needs[1] = Integer.parseInt(s);
                     s = t.nextToken();
                     birds[g].needs[2] = Integer.parseInt(s);
                     s = t.nextToken();
                     birds[g].needs[3] = Integer.parseInt(s);
                  }
                  else
                  {
                     s = t.nextToken();
                     birds[g].needs[0] = Integer.parseInt(s);
                     s = t.nextToken();
                     birds[g].needs[1] = Integer.parseInt(s);
                  }
               }
               else if (s.equals("response"))
               {
                  s = t.nextToken();
                  birds[g].response = Integer.parseInt(s);
                  responseNumber[g]++;
               }
               else
               {
                  throw(new IOException());
               }
            }
         }
      }
      catch (NoSuchElementException e) {
         status("Invalid line: '" + l + "' in source " + source);
         System.exit(1);
      }
      catch (NumberFormatException e) {
         status("Invalid numeric value in line: '" + l + "' in source " + source);
         System.exit(1);
      }
      catch (IOException e) {
         status("IO error reading source " + source);
         System.exit(1);
      }

      // EOF on source.
      sourceReader = null;
      status("That's all, folks!");
   }


   // Update display.
   public void updateDisplay()
   {
      int    x, y, x2, y2, i, j;
      String s;

      if (worldCreated == false) { return; }

      // Show readme?
      if (viewChoice.getSelectedIndex() == README)
      {
         worldImageGraphics.setColor(Color.white);
         worldImageGraphics.fillRect(0, 0, worldImageSize.width, worldImageSize.height);
         worldImageGraphics.setColor(Color.black);
         for (i = 0, j = fontHeight; i < readme.length; i++, j += fontHeight)
         {
            s = readme[i];
            worldImageGraphics.drawString(s, fontWidth, j);
         }
         canvasGraphics.drawImage(worldImage, 0, 0, this);
         return;
      }

      // Draw bird network?
      if (viewChoice.getSelectedIndex() == MALE_NET)
      {
         if (maleNetImageLoaded != responseNumber[MALE])
         {
            loadBirdNetImage(MALE, responseNumber[MALE]);
         }
         canvasGraphics.drawImage(maleNetImage, 0, 0, this);
         return;
      }
      if (viewChoice.getSelectedIndex() == FEMALE_NET)
      {
         if (femaleNetImageLoaded != responseNumber[FEMALE])
         {
            loadBirdNetImage(FEMALE, responseNumber[FEMALE]);
         }
         canvasGraphics.drawImage(femaleNetImage, 0, 0, this);
         return;
      }

      // Clear.
      worldImageGraphics.setColor(Color.white);
      worldImageGraphics.fillRect(0, 0, worldImageSize.width, worldImageSize.height);

      // Draw world.
      worldImageGraphics.setColor(Color.black);
      y2 = worldImageSize.height;
      int h = STATUS_PANEL_SIZE.height;
      for (x = x2 = 0; x < worldSize.width; x++, x2 += CELL_SIZE.width)
      {
         worldImageGraphics.drawLine(x2, h, x2, y2);
      }
      worldImageGraphics.drawLine(x2, h, x2, y2);
      x2 = worldSize.width * CELL_SIZE.width;
      for (y = 0, y2 = h; y < worldSize.width; y++, y2 += CELL_SIZE.height)
      {
         worldImageGraphics.drawLine(0, y2, x2, y2);
      }
      worldImageGraphics.drawLine(0, y2, x2, y2);
      for (x = x2 = 0; x < worldSize.width; x++, x2 += CELL_SIZE.width)
      {
         for (y = 0, y2 = h; y < worldSize.height; y++, y2 += CELL_SIZE.height)
         {
            switch (world[x][y].locale)
            {
            case DESERT:
               worldImageGraphics.drawImage(cactusImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case FOREST:
               worldImageGraphics.drawImage(treeImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case GRASSLAND:
               break;
            }

            switch (world[x][y].object)
            {
            case NO_OBJECT:
               break;

            case EGG:
               worldImageGraphics.drawImage(eggImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case FOOD:
               worldImageGraphics.drawImage(mouseImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case STONE:
               worldImageGraphics.drawImage(stoneImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;
            }
         }
      }

      // Female status.
      worldImageGraphics.drawImage(femaleImage, femaleImageLocation.x,
                                   femaleImageLocation.y, this);
      s = "Female";
      x = femaleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = femaleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (birds[FEMALE].hasObject == FOOD)
      {
         worldImageGraphics.drawImage(mouseImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      else if (birds[FEMALE].hasObject == STONE)
      {
         worldImageGraphics.drawImage(stoneImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      s = "Response (#" + responseNumber[FEMALE] + "): " +
          responseToString(birds[FEMALE].response);
      x = femaleStatusLocation.x + statusInfoOffset.x;
      y = femaleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s = "Food: " + birds[FEMALE].food;
      i = fontAscent + 2;
      y = femaleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Need food: " + birds[FEMALE].needs[0];
      i += fontAscent + 2;
      y  = femaleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Need egg in nest: " + birds[FEMALE].needs[1];
      i += fontAscent + 2;
      y  = femaleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      x2 = birds[FEMALE].location.x * CELL_SIZE.width;
      y2 = (birds[FEMALE].location.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(femaleSmallImage, x2 + femaleImageOffset.x,
                                   y2 + femaleImageOffset.y, this);

      // Male status.
      worldImageGraphics.drawImage(maleImage, maleImageLocation.x,
                                   maleImageLocation.y, this);
      s = "Male";
      x = maleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = maleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (birds[MALE].hasObject == FOOD)
      {
         worldImageGraphics.drawImage(mouseImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      else if (birds[MALE].hasObject == STONE)
      {
         worldImageGraphics.drawImage(stoneImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      s = "Response (#" + responseNumber[MALE] + "): " +
          responseToString(birds[MALE].response);
      x = maleStatusLocation.x + statusInfoOffset.x;
      y = maleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s = "Food: " + birds[MALE].food;
      i = fontAscent + 2;
      y = maleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Need food: " + birds[MALE].needs[0];
      i += fontAscent + 2;
      y  = maleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Mate needs food: " + birds[MALE].needs[1];
      i += fontAscent + 2;
      y  = maleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Mate needs stone: " + birds[MALE].needs[2];
      i += fontAscent + 2;
      y  = maleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      s  = "Need to check mate: " + birds[MALE].needs[3];
      i += fontAscent + 2;
      y  = maleStatusLocation.y + statusInfoOffset.y + i;
      worldImageGraphics.drawString(s, x, y);
      x2 = birds[MALE].location.x * CELL_SIZE.width;
      y2 = (birds[MALE].location.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(maleSmallImage, x2 + maleImageOffset.x,
                                   y2 + maleImageOffset.y, this);

      // Copy to display.
      canvasGraphics.drawImage(worldImage, 0, 0, this);
   }


   // Response to string.
   public String responseToString(int response)
   {
      switch (response)
      {
      case DO_NOTHING:
         return("DO_NOTHING");

      case EAT:
         return("EAT");

      case GET:
         return("GET");

      case GO_TO_DESERT:
         return("GO_TO_DESERT");

      case GO_TO_FOREST:
         return("GO_TO_FOREST");

      case GO_TO_MATE:
         return("GO_TO_MATE");

      case LAY_EGG:
         return("LAY_EGG");

      case LOOK_FOR_FOOD:
         return("LOOK_FOR_FOOD");

      case LOOK_FOR_STONE:
         return("LOOK_FOR_STONE");

      case PUT:
         return("PUT");

      case RECEIVE:
         return("RECEIVE");

      case STEP:
         return("STEP");

      case TOSS:
         return("TOSS");

      case TURN:
         return("TURN");

      case WANT_STONE_SET:
         return("WANT_STONE_SET");

      case WANT_STONE_UNSET:
         return("WANT_STONE_UNSET");

      default:
         return("");
      }
   }


   // View choice listener.
   class viewChoiceItemListener implements ItemListener
   {
      public void itemStateChanged(ItemEvent evt)
      {
         // Clear canvas for new image.
         canvasGraphics.setColor(Color.white);
         canvasGraphics.fillRect(0, 0, CANVAS_SIZE.width, CANVAS_SIZE.height);
      }
   }

   // Response slider listener.
   class responseSliderListener implements ChangeListener
   {
      public void stateChanged(ChangeEvent evt)
      {
         response = responseSlider.getValue();
      }
   }

   // Load bird network image.
   void loadBirdNetImage(int gender, int index)
   {
      URL          u;
      Image        image;
      MediaTracker tracker;
      String       imageName;

      if (gender == MALE)
      {
         maleNetImageLoaded = index;
         imageName          = "male" + index + ".jpg";
      }
      else
      {
         femaleNetImageLoaded = index;
         imageName            = "female" + index + ".jpg";
      }
      tracker = new MediaTracker(this);
      image   = loadImage(imageName);
      tracker.addImage(image, 0);
      if (gender == MALE)
      {
         maleNetImage = image;
      }
      else
      {
         femaleNetImage = image;
      }
      try
      {
         tracker.waitForAll();
      }
      catch (InterruptedException e) {
         status("Image loading interrupted");
      }
   }


   // Load image from file or URL.
   Image loadImage(String name)
   {
      Image image;

      String filename = "images/" + name;

      try {
         image = ImageIO.read(new File(filename));
      }
      catch (IOException e) {
         image = null;
      }
      if (image == null)
      {
         try
         {
            URL u = new URL(getCodeBase(), filename);
            image = getImage(u);
         }
         catch (MalformedURLException e) {
            status("Bad URL for image " + name);
            System.exit(1);
         }
      }
      return(image);
   }


   // Status message.
   void status(String message)
   {
      try
      {
         showStatus(message);
      }
      catch (Exception e)
      {
         System.out.println(message);
      }
   }


   // Main.
   @SuppressWarnings("deprecation")
   public static void main(String[] args)
   {
      // Create nest viewer.
      NestViewer nestViewer = new NestViewer();

      // Set source.
      nestViewer.source = NestViewer.DEFAULT_SOURCE;
      if (args.length == 1)
      {
         nestViewer.source = args[0];
      }
      else if (args.length == 2)
      {
         nestViewer.source = args[1];
      }
      else if (args.length != 0)
      {
         System.err.println("Usage: java mona.NestViewer [<source file name>]");
         System.exit(1);
      }

      // Create frame.
      JFrame frame = new JFrame();
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.setTitle("Nesting birds");
      frame.setBounds(0, 0, 667, 655);
      frame.setLayout(new GridLayout(1, 1));
      frame.add(nestViewer);
      frame.setVisible(true);

      // Run applet.
      nestViewer.init();
      nestViewer.start();
      frame.resize(new Dimension(668, 655));
   }
}
