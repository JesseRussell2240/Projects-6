document.addEventListener("DOMContentLoaded", function () {
    // Smooth scrolling
    const links = document.querySelectorAll('.subtab-nav a[href^="#"]');
    links.forEach(link => {
        link.addEventListener("click", function (event) {
            event.preventDefault();
            const targetId = this.getAttribute("href").substring(1);
            const targetElement = document.getElementById(targetId);
            if (targetElement) {
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

    // Persist checkboxes
    const checkboxes = document.querySelectorAll(".task-check");
    checkboxes.forEach((box, index) => {
        const saved = localStorage.getItem(`task-${index}`);
        if (saved === "true") box.checked = true;
        box.addEventListener("change", () => {
            localStorage.setItem(`task-${index}`, box.checked);
        });
    });

    // Username char countdown
    const usernameInput = document.getElementById("username");
    const charCountDisplay = document.getElementById("char-count");
    const maxChars = 180;
    if (usernameInput && charCountDisplay) {
        usernameInput.addEventListener("input", function () {
            const remaining = maxChars - usernameInput.value.length;
            charCountDisplay.textContent = `${remaining} characters remaining`;
        });
    }
});

    window.onload = function () {
  const userInput = document.querySelector('input[name="username"]');
  if (userInput) {
    userInput.focus();
  }
};

