document.addEventListener("DOMContentLoaded", function() {
  // Select all subtab nav links with hash href
  const links = document.querySelectorAll('.subtab-nav a[href^="#"]');

  links.forEach(link => {
    link.addEventListener("click", function(event) {
      event.preventDefault();

      const targetId = this.getAttribute("href").substring(1);
      const targetElement = document.getElementById(targetId);

      if (targetElement) {
        // Calculate offset to avoid fixed nav overlap (adjust 80 as needed)
        const navOffset = 80;
        const elementPosition = targetElement.getBoundingClientRect().top;
        const offsetPosition = elementPosition + window.pageYOffset - navOffset;

        window.scrollTo({
          top: offsetPosition,
          behavior: "smooth"
        });
      }
    });
  });
});

document.addEventListener("DOMContentLoaded", () => {
  const checkboxes = document.querySelectorAll(".task-check");

  // Load saved state
  checkboxes.forEach((box, index) => {
    const saved = localStorage.getItem(`task-${index}`);
    if (saved === "true") {
      box.checked = true;
    }

    // Save on click
    box.addEventListener("change", () => {
      localStorage.setItem(`task-${index}`, box.checked);
    });
  });
});

window.addEventListener("scroll", () => {
  const scrollTop = window.scrollY;
  const docHeight = document.documentElement.scrollHeight - window.innerHeight;
  const scrollPercent = (scrollTop / docHeight) * 100;

  const progressBar = document.getElementById("scroll-progress-bar");
  const scrollCar = document.getElementById("scroll-car");

  const viewportWidth = window.innerWidth;

  if (progressBar) {
    progressBar.style.width = scrollPercent + "%";
  }

  if (scrollCar) {
    const carWidth = scrollCar.offsetWidth;
    const carX = (scrollPercent / 100) * viewportWidth;

    scrollCar.style.left = `${Math.min(carX, viewportWidth - carWidth)}px`;

  }
});

function toggleIframe(iframeId, buttonId) {
  
  const iframe = document.getElementById(iframeId);
  const button = document.getElementById(buttonId);
  
   if (iframe.style.display === "none" || iframe.style.display === "") {
      iframe.style.display = "block";
      button.innerText = "Hide " + button.innerText.replace("Show ", "");
    } else {
      iframe.style.display = "none";
      button.innerText = "Show " + button.innerText.replace("Hide ", "");
    }
  
}

function updateDateTime() {
    const now = new Date();
    const formatted = now.toLocaleString();
    document.getElementById("datetime").textContent = "Current Date & Time: " + formatted;
}

// Call it on page load
document.addEventListener("DOMContentLoaded", () => {
    updateDateTime();
    setInterval(updateDateTime, 1000);
});


window.addEventListener('DOMContentLoaded', () => {
  const notification = document.querySelector('.overlay-notification');
  if (notification) {
    setTimeout(() => {
      notification.style.opacity = '0';
      notification.style.transition = 'opacity 0.5s ease';
      setTimeout(() => notification.remove(), 500);
    }, 4000);
  }
});





