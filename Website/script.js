/* === GLOBAL BODY STYLES === */
body {
  background-color: #000;           /* Full black background for dark theme */
  color: #ffd700;                   /* Default text color set to gold */
  font-family: Arial, sans-serif;  /* Clean and web-safe font */
  margin: 0;                        /* Remove default margin */
  padding: 0;                       /* Remove default padding */
  scroll-behavior: smooth;         /* Smooth scrolling for anchor links */
  padding-top: 120px;               /* Prevent content from hiding behind the fixed top banner */
}

/* === GLOBAL LINK STYLES === */
a {
  color: #cd7f32;                  /* Bronze color for default links */
  text-decoration: none;          /* Remove underline by default */
}

a:hover {
  color: #ffd700;                 /* Gold on hover */
  text-decoration: underline;    /* Re-add underline on hover */
}

/* === MAIN NAVIGATION BAR === */
nav {
  background-color: #111;         /* Dark gray background for nav */
  display: flex;                  /* Use flexbox for layout */
  justify-content: space-around; /* Distribute nav items evenly */
  padding: 10px;                  /* Spacing inside nav */
}

nav a {
  color: #cd7f32;                 /* Bronze links in nav */
  text-decoration: none;
  font-weight: bold;             /* Emphasize nav links */
  padding: 8px 12px;             /* Padding around nav items */
  transition: background-color 0.3s ease; /* Smooth hover effect */
}

nav a:hover {
  background-color: #222;        /* Slightly lighter black on hover */
  border-radius: 5px;            /* Rounded corners */
}

/* === ACTIVE NAV LINK STYLES === */
nav a.active {
  background-color: #1a1a00;     /* Slightly tinted background for active tab */
  color: #ffd700;                /* Bright gold to highlight */
  border-radius: 5px;            /* Rounded corners */
  font-weight: bold;
  border-bottom: 2px solid #ffd700; /* Gold underline for visual cue */
}

/* === FIXED TOP BANNER === */
#top-banner {
  position: fixed;               /* Keep at top during scroll */
  top: 0;
  width: 100%;
  background-color: #111;        /* Match nav bar color */
  color: #ffd700;                /* Gold text */
  display: flex;                 /* Flexbox layout for items in banner */
  flex-direction: column;
  align-items: start;
  justify-content: start;
  padding: 10px 20px;
  z-index: 1000;                 /* Stay above all other elements */
  /*border-bottom: none; */
  border-bottom: 2px solid #cd7f32;  /* Bronze border for style */ 
  box-shadow: 0 2px 6px rgba(0, 0, 0, 0.5); /* Subtle shadow for depth */
  position: fixed;
}

/* === LOGO STYLING INSIDE BANNER === */
#logo {
  height: 40px;                  /* Scaled logo height */
  margin-right: 15px;            /* Spacing between logo and title/text */
}

/* === SCROLL BAR === */
#scroll-progress-bar {
  position: absolute;
  bottom: 0;
  left: 0;
  height: 4px;
  background-color: #ffd700;
  z-index: 1001;
  width: 0%; 
  overflow: visible;
}

#scroll-car {
  position: absolute;
  top: -10px;         
  height: 20px;        
  left: 0;
  pointer-events: none;
  transition: left 0.05s linear;
}


/* === SUBTAB NAVIGATION CONTAINER === */
.subtab-nav {
  margin: 20px 0;                /* Space above and below subtab bar */
  display: flex;
  gap: 10px;                     /* Spacing between subtabs */
  flex-wrap: wrap;              /* Wrap subtabs on smaller screens */
}

/* === SUBTAB BUTTON STYLES === */
.subtab-nav a {
  padding: 10px 20px;
  background-color: #111;        /* Dark black background for buttons */
  border: 2px solid #ffd700;     /* Gold border for visual appeal */
  color: #ffd700;                /* Gold text */
  text-decoration: none;
  border-radius: 8px;            /* Rounded corners */
  font-weight: bold;
  cursor: pointer;
  transition: background-color 0.3s ease, color 0.3s ease;
}

.subtab-nav a:hover,
.subtab-nav a:focus {
  background-color: #ffd700;     /* Gold background on hover/focus */
  color: #111;                   /* Inverted text color */
  outline: none;                 /* Remove default focus outline */
}

/* === CONTENT SECTIONS FOR SUBTABS === */
section.subtab-section {
  padding: 40px 20px;            /* Internal spacing */
  border-bottom: 1px solid #ffd700; /* Gold bottom border for separation */
  margin-bottom: 40px;           /* Space before next section */
}

/* === TARGET OFFSET FIX FOR FIXED NAVIGATION === */
section.subtab-section:target {
  padding-top: 80px;             /* Extra space at top when linked via anchor */
  margin-top: -80px;             /* Pull back up to maintain visual alignment */
}

/* === STYLING FOR SCHEDULE TABLE === */
#schedule {
  width: 100%;                   /* Full width */
  border-collapse: collapse;     /* Remove spacing between borders */
  margin-top: 20px;
}

#schedule th, #schedule td {
  border: 1px solid #cd7f32;     /* Bronze border */
  padding: 10px;                 /* Internal cell padding */
  text-align: left;              /* Align text to left */
}

#schedule th {
  background-color: #111;        /* Dark header */
  color: #ffd700;                /* Gold text */
}

#schedule td {
  background-color: #000;        /* Black background for cells */
  color: #ffd700;                /* Gold text */
}

/* === STYLING FOR CHECKBOX ELEMENTS === */
input[type="checkbox"] {
  transform: scale(1.2);         /* Make checkboxes larger */
  cursor: pointer;               /* Show pointer on hover */
}
