const currentDate = new Date();
// Subtract one day to get yesterday's date
const yesterdayDate = new Date(currentDate);
yesterdayDate.setDate(currentDate.getDate() - 1);

const monthName = yesterdayDate.toLocaleDateString("en-US", { month: "long" }); // e.g., "May"
const dayOfMonth = yesterdayDate.getDate(); // e.g., 28
const formattedDateText = monthName + " " + dayOfMonth;

// Generate fromDate (start of yesterday in UTC)
const fromDate = new Date(yesterdayDate);
fromDate.setUTCHours(0, 0, 0, 0); // Set to start of day in UTC
const fromDateISO = fromDate.toISOString(); // e.g., "2025-01-27T00:00:00.000Z"

// Generate toDate (end of yesterday in UTC)
const toDate = new Date(yesterdayDate);
toDate.setUTCHours(23, 59, 59, 999); // Set to end of day in UTC
const toDateISO = toDate.toISOString(); // e.g., "2025-01-27T23:59:59.999Z"

// n8n expects an array of objects.
// Each object should have a 'json' property where you put your data.
// If you have incoming items and want to add to them:
if (items && items.length > 0) {
  items[0].json.fromDate = fromDateISO;
  items[0].json.toDate = toDateISO;
  items[0].json.formattedDate = formattedDateText;
  // If you want to add it to all incoming items:
  // for (const item of items) {
  //   item.json.fromDate = fromDateISO;
  //   item.json.toDate = toDateISO;
  // }
} else {
  // If there are no incoming items, or you want to create a new item:
  items = [{ json: { fromDate: fromDateISO, toDate: toDateISO } }];
}

return items;
