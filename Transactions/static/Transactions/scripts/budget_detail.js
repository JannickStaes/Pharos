//click to edit Budget
const displayBudget = document.querySelector('#displayBudget');
const editBudget = document.querySelector('#editBudget');
const editButton = document.querySelector('#budgetEditButton');
const newBudget = document.querySelector('#newBudget');
const newBudgetButton = document.querySelector('#newBudgetButton')

displayBudget.parentNode.removeChild(editBudget);
editButton.addEventListener('click', changeToEdit);
newBudgetButton.addEventListener('click', saveNewBudget);

function changeToEdit() {
    displayBudget.parentNode.appendChild(editBudget);
    displayBudget.parentNode.removeChild(displayBudget);
    editBudget.style.margin = 'auto';
}

function saveNewBudget() {
  const csrfToken = getCSRFToken();
  const currentURL = window.location.href;
  const url = currentURL + '/update_target/'; 
  const new_budget = newBudget.value;
  const data = {
    new_budget
  };

  fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-CSRFToken': csrfToken, 
    },
    body: JSON.stringify(data)
  })
    .then(response => response.json()) // Parse the response as JSON
    .then(data => {
      if(data.redirect) {
        window.location.href = data.redirect;
      }
    })
    .catch(error => {
      console.error('Error:', error);
    });
}

function getCSRFToken() {
  const csrfCookie = document.cookie.split(';').find(cookie => cookie.trim().startsWith("csrftoken="));
  if (csrfCookie) {
    return csrfCookie.split('=')[1];
  }
  return null;
}

  