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



