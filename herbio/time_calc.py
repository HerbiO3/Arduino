import datetime

# Define the reference time as a datetime object
REF_TIME = datetime.datetime(2000, 1, 1)

def iso8601_to_seconds(iso_string):
    # Convert the ISO 8601 string to a datetime object
    dt = datetime.datetime.fromisoformat(iso_string)

    # Calculate the difference between the reference time and the datetime object
    diff = dt - REF_TIME

    # Convert the difference to seconds
    seconds = int(diff.total_seconds())

    return seconds

def current_time_to_iso8601():
    # Get the current system time as a datetime object
    now = datetime.datetime.now()

    # Convert the datetime object to an ISO 8601 string
    iso_string = now.isoformat()

    return iso_string[:19]

def current_time_to_seconds():
    # Get the current system time as a datetime object
    now = datetime.datetime.now()

    # Calculate the difference between the reference time and the datetime object
    diff = now - REF_TIME

    # Convert the difference to seconds
    seconds = int(diff.total_seconds())

    return seconds

def add_seconds_to_time(seconds):
    # Calculate the new datetime object by adding the seconds to the current system time
    new_time = datetime.datetime.now() + datetime.timedelta(seconds=seconds)

    # Convert the datetime object to an ISO 8601 string
    iso_string = new_time.isoformat()

    return iso_string

def iso8601_to_seconds(iso_string):
    # Convert the ISO 8601 string to a datetime object
    dt = datetime.datetime.fromisoformat(iso_string)

    # Calculate the difference between the reference time and the datetime object
    diff = dt - REF_TIME

    # Convert the difference to seconds
    seconds = int(diff.total_seconds())

    return seconds

# Example usage:
print(f"current time: {current_time_to_iso8601()}")
diffs = 90
print(f"current time : {current_time_to_seconds()}")
print(f"current time + {diffs}: {current_time_to_seconds()+diffs}")
