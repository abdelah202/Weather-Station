function collectDataFromThingSpeak() {
  // ThingSpeak channel details
  var channelID = ' 2563429';
  var readAPIKey = '7YO1U3SYWUXFP6D9';
  var numResults = 10; // Number of results to fetch

  // Google Sheets details
  var spreadsheetID = '1zO-0VCxQZ3zRA6ue8dJcqxY8p0DNBvvKVxX_QljIZsY';
  var sheetName = 'Sheet1';
  var sheet = SpreadsheetApp.openById(spreadsheetID).getSheetByName(sheetName);

  // If the sheet doesn't exist, create it
  if (!sheet) {
    var spreadsheet = SpreadsheetApp.openById(spreadsheetID);
    sheet = spreadsheet.insertSheet(sheetName);
  }

  // Append data to Google Sheets
  sheet.appendRow([timestamp, temperature, humidity]);

  // Fetch data from ThingSpeak
  var url = 'https://api.thingspeak.com/channels/' + channelID + '/feeds.json?api_key=' + readAPIKey + '&results=' + numResults;
  var response = UrlFetchApp.fetch(url);
  var json = response.getContentText();
  var data = JSON.parse(json);

  // Extract relevant data and write to Google Sheets
  var sheet = SpreadsheetApp.openById(spreadsheetID).getSheetByName(sheetName);
  var rows = data.feeds.length;

  for (var i = 0; i < rows; i++) {
    var row = data.feeds[i];
    var timestamp = new Date(row.created_at);
    var temperature = parseFloat(row.field1);
    var humidity = parseFloat(row.field2);

    // Append data to Google Sheets
    sheet.appendRow([timestamp, temperature, humidity]);
  }
}
