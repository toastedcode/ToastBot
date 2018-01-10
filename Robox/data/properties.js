function updateProperty(inputId)
{
   propertyName = document.getElementById(inputId).name;
   propertyValue = document.getElementById(inputId).value;

    form = document.createElement('form');
  form.setAttribute('method', 'GET');

  input = document.createElement('input');
  input.setAttribute('name', 'action');
  input.setAttribute('value', 'update');
  form.appendChild(input);

  input = document.createElement('input');
  input.setAttribute('name', 'propertyName');
  input.setAttribute('value', propertyName);
  form.appendChild(input);

  input = document.createElement('input');
  input.setAttribute('name', 'propertyValue');
  input.setAttribute('value', propertyValue);
  form.appendChild(input);

  document.body.appendChild(form);
    form.submit();
}

function deleteProperty(inputId)
{
   propertyName = document.getElementById(inputId).name;
   propertyValue = document.getElementById(inputId).value;

    form = document.createElement('form');
  form.setAttribute('method', 'GET');

  input = document.createElement('input');
  input.setAttribute('name', 'action');
  input.setAttribute('value', 'delete');
  form.appendChild(input);

  input = document.createElement('input');
  input.setAttribute('name', 'propertyName');
  input.setAttribute('value', propertyName);
  form.appendChild(input);

  document.body.appendChild(form);
    form.submit();
}

function addProperty(nameInputId, valueInputId)
{
   propertyName = document.getElementById(nameInputId).value;
   propertyValue = document.getElementById(valueInputId).value;

    form = document.createElement('form');
  form.setAttribute('method', 'GET');

  input = document.createElement('input');
  input.setAttribute('name', 'action');
  input.setAttribute('value', 'update');
  form.appendChild(input);

  input = document.createElement('input');
  input.setAttribute('name', 'propertyName');
  input.setAttribute('value', propertyName);
  form.appendChild(input);

  input = document.createElement('input');
  input.setAttribute('name', 'propertyValue');
  input.setAttribute('value', propertyValue);
  form.appendChild(input);

  document.body.appendChild(form);
    form.submit();
}

function reset()
{
   form = document.createElement('form');
   form.setAttribute('method', 'GET');

   input = document.createElement('input');
   input.setAttribute('name', 'action');
   input.setAttribute('value', 'reset');
   form.appendChild(input);

   document.body.appendChild(form);
   form.submit();
}